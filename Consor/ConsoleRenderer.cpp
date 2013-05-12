#include "ConsoleRenderer.hpp"
#include <assert.h>

using namespace std;

using namespace Consor;
using namespace Consor::Console;

string IConsoleRenderer::RendererName()
{
	return "AbstractConsoleRenderer";
}

void IConsoleRenderer::Clear(const CColour& col)
{
	DrawBox(CVector(), Size(), col);
}

void IConsoleRenderer::DrawBox(const CVector& pos, const CSize& size, const CColour& col)
{
	char box = ' ';

	for(int x = pos.X; x < pos.X + size.Width; x++)
		for(int y = pos.Y; y < pos.Y + size.Height; y++)
		{
			unique_ptr<ICharInformation> info = GetCharInformation(CVector(x, y));
			info->SetChar(box);
			info->SetBackgroundColour(col);
		}
}

void IConsoleRenderer::DrawString(const std::string& str, const CVector& Pos, const CColour& fgcol, const CColour& bgcol)
{
	CVector pos = Pos;

	for(const char& letter : str)
	{
		if(letter == '\n')
		{
			pos.X = Pos.X;
			pos.Y++;
			continue;
		}

		unique_ptr<ICharInformation> info = GetCharInformation(pos);

		info->SetChar(letter);
		info->SetBackgroundColour( CColour::Blend(bgcol, info->GetBackgroundColour()) );
		info->SetForegroundColour( CColour::Blend(fgcol, info->GetForegroundColour()) );
		
		pos.X++;
	}
}

void IConsoleRenderer::PushRenderBounds(const CVector& from, const CSize& size)
{
	renderbound_t rb;

	//rb.Pos = from + m_CurrentRenderBound.Pos; // this is added as the new renderbounds will apear to be the full console size
	rb.Pos = from + m_CurrentOffset; // this should be correc,t but not fully tested...
	rb.Size = size;

	CVector offset = rb.Pos;
	
	if(m_Bounds.size() != 0)
	{
		if(rb.Pos.X < m_CurrentRenderBound.Pos.X)
			rb.Pos.X = m_CurrentRenderBound.Pos.X;
		if(rb.Pos.X > m_CurrentRenderBound.Pos.X + m_CurrentRenderBound.Size.Width)
			rb.Size.Width = 0; // too far over to the right...
		if(rb.Pos.X + rb.Size.Width > m_CurrentRenderBound.Pos.X + m_CurrentRenderBound.Size.Width)
			rb.Size.Width = (m_CurrentRenderBound.Pos.X + m_CurrentRenderBound.Size.Width) - rb.Pos.X;

		if(rb.Pos.Y < m_CurrentRenderBound.Pos.Y)
			rb.Pos.Y = m_CurrentRenderBound.Pos.Y;
		if(rb.Pos.Y > m_CurrentRenderBound.Pos.Y + m_CurrentRenderBound.Size.Height)
			rb.Size.Height = 0; // too far over to the right...
		if(rb.Pos.Y + rb.Size.Height > m_CurrentRenderBound.Pos.Y + m_CurrentRenderBound.Size.Height)
			rb.Size.Height = (m_CurrentRenderBound.Pos.Y + m_CurrentRenderBound.Size.Height) - rb.Pos.Y;
	}

	m_Bounds.push_back(rb);
	m_Offsets.push_back(offset);

	m_CurrentRenderBound = rb;
	m_CurrentOffset = offset;
}

void IConsoleRenderer::PopRenderBounds()
{
	m_Bounds.pop_back();
	m_Offsets.pop_back();

	assert(m_Bounds.size() > 0);

	m_CurrentRenderBound = m_Bounds.back(); // last element
	m_CurrentOffset = m_Offsets.back();
}

CSize IConsoleRenderer::RenderSize()
{
	return CSize(m_CurrentRenderBound.Size.Width, m_CurrentRenderBound.Size.Height);
}


/*
 MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY
 MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY
 MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY
 MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY
 MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY
*/



class CRectangleChar
{
	int m_State;
public:
	const static int LEFT	= (1 << 0);
	const static int RIGHT	= (1 << 1);
	const static int UP		= (1 << 2);
	const static int DOWN	= (1 << 3);
	
	CRectangleChar(const bool left, const bool right, const bool up, const bool down)
	{
		m_State = 0;

		if(left)
			m_State |= LEFT;
		if(right)
			m_State |= RIGHT;
		if(up)
			m_State |= UP;
		if(down)
			m_State |= DOWN;
	}
		
	CRectangleChar(unsigned char type)
	{
		#define CHAR_STATE(ltr, val) \
		case ltr: \
			m_State = (val); \
			break

		switch(type)
		{
			CHAR_STATE(1, LEFT);
			CHAR_STATE(2, RIGHT);
			CHAR_STATE(3, UP);
			CHAR_STATE(4, DOWN);
			
			CHAR_STATE(179, UP | DOWN);
			CHAR_STATE(180, UP | DOWN | LEFT);
			CHAR_STATE(191, DOWN | LEFT);
			CHAR_STATE(192, UP | RIGHT);
			CHAR_STATE(193, LEFT | UP | RIGHT);
			CHAR_STATE(194, LEFT | DOWN | RIGHT);
			CHAR_STATE(195, UP | DOWN | RIGHT);
			CHAR_STATE(196, LEFT | RIGHT);
			CHAR_STATE(197, LEFT | RIGHT | UP | DOWN);
			CHAR_STATE(217, UP | LEFT);
			CHAR_STATE(218, RIGHT | DOWN);

		default:
			m_State = 0;
		}

		#undef CHAR_STATE
	}

	char GetChar()
	{
		 // same as the one up there ^, but the only difference is it translated the other way
		#define CHAR_STATE(ltr, val) \
		case (val): \
			return (char)ltr;

		switch(m_State)
		{
			CHAR_STATE(1, LEFT);
			CHAR_STATE(2, RIGHT);
			CHAR_STATE(3, UP);
			CHAR_STATE(4, DOWN);

			CHAR_STATE(179, UP | DOWN);
			CHAR_STATE(180, UP | DOWN | LEFT);
			CHAR_STATE(191, DOWN | LEFT);
			CHAR_STATE(192, UP | RIGHT);
			CHAR_STATE(193, LEFT | UP | RIGHT);
			CHAR_STATE(194, LEFT | DOWN | RIGHT);
			CHAR_STATE(195, UP | DOWN | RIGHT);
			CHAR_STATE(196, LEFT | RIGHT);
			CHAR_STATE(197, LEFT | RIGHT | UP | DOWN);
			CHAR_STATE(217, UP | LEFT);
			CHAR_STATE(218, RIGHT | DOWN);

		default:
			return '?';
		}

		#undef CHAR_STATE
	}

	CRectangleChar operator+(const CRectangleChar& other)
	{
		CRectangleChar ret(0);
		ret.m_State = this->m_State | other.m_State;

		return ret;
	}

	CRectangleChar operator-(const CRectangleChar& other)
	{
		CRectangleChar ret(0);
		ret.m_State = this->m_State & ~other.m_State;

		return ret;
	}
};

void IConsoleRenderer::DrawRect(const CVector& pos, const CSize& size, const CColour& fgcol, const CColour& bgcol)
{
	CRectangleChar Horizontal(true, true, false, false);
	CRectangleChar Vertical(false, false, true, true);

	CRectangleChar Left(true, false, false, false);
	CRectangleChar Right(false, true, false, false);

	CRectangleChar Top(false, false, true, false);
	CRectangleChar Bottom(false, false, false, true);
	
	if(size.Width == 1)
		;
	else if(size.Height == 1)
		for(int x = pos.X; x < pos.X + size.Width; x++)
		{
			unique_ptr<ICharInformation> info_top = GetCharInformation(CVector(x, pos.Y));
			
			info_top->SetChar((CRectangleChar(info_top->GetChar()) + Horizontal).GetChar());
			info_top->SetForegroundColour(CColour::Blend(fgcol, info_top->GetForegroundColour()));
			info_top->SetBackgroundColour(CColour::Blend(bgcol, info_top->GetBackgroundColour()));
		}
	else
	for(int x = pos.X; x < pos.X + size.Width; x++)
	{
		unique_ptr<ICharInformation> info_top = GetCharInformation(CVector(x, pos.Y));
		unique_ptr<ICharInformation> info_bot = GetCharInformation(CVector(x, pos.Y + size.Height - 1));

		CRectangleChar cur = Horizontal;

		if(x == pos.X)
			cur = cur - Left;
		else if(x == pos.X + size.Width - 1)
			cur = cur - Right;

		info_top->SetChar((CRectangleChar(info_top->GetChar()) + cur).GetChar());
		info_top->SetForegroundColour(fgcol);
		info_top->SetBackgroundColour(bgcol);

		info_bot->SetChar((CRectangleChar(info_bot->GetChar()) + cur).GetChar());
		info_bot->SetForegroundColour(fgcol);
		info_bot->SetBackgroundColour(bgcol);
	}

	if(size.Height == 1)
		;
	else if(size.Width == 1)
		for(int y = pos.Y; y < pos.Y + size.Height; y++)
		{
			unique_ptr<ICharInformation> info_l = GetCharInformation(CVector(pos.X, y));

			info_l->SetChar( (CRectangleChar(info_l->GetChar()) + Vertical).GetChar() );
			info_l->SetForegroundColour(fgcol);
			info_l->SetBackgroundColour(bgcol);
		}
	else
	for(int y = pos.Y; y < pos.Y + size.Height; y++)
	{
		unique_ptr<ICharInformation> info_l = GetCharInformation(CVector(pos.X, y));
		unique_ptr<ICharInformation> info_r = GetCharInformation(CVector(pos.X + size.Width - 1, y));

		CRectangleChar cur = Vertical;

		if(y == pos.Y)
			cur = cur - Top;
		else if(y == pos.Y + size.Height - 1)
			cur = cur - Bottom;

		info_l->SetChar( (CRectangleChar(info_l->GetChar()) + cur).GetChar() );
		info_l->SetForegroundColour(fgcol);
		info_l->SetBackgroundColour(bgcol);

		info_r->SetChar( (CRectangleChar(info_r->GetChar()) + cur).GetChar() );
		info_r->SetForegroundColour(fgcol);
		info_r->SetBackgroundColour(bgcol);
	}
}