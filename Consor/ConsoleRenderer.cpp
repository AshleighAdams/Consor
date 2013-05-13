#include "ConsoleRenderer.hpp"
#include <assert.h>
#include <locale>
#include <codecvt>

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

	unique_ptr<ICharInformation> info = GetCharInformation(CVector());

	for(int x = pos.X; x < pos.X + size.Width; x++)
		for(int y = pos.Y; y < pos.Y + size.Height; y++)
		{
			info->SetPosition(CVector(x, y));
			info->SetChar(box);
			info->SetBackgroundColour(col);
		}
}
/*
bool IConsoleRenderer::SupportsUnicode()
{
	return true;
}
*/

void IConsoleRenderer::DrawString(const std::string& str, const CVector& Pos, const CColour& fgcol, const CColour& bgcol)
{
	CVector pos = Pos;
	u32string unistr = wstring_convert<codecvt_utf8_utf16<char32_t>, char32_t>().from_bytes(str);

	bool unicode = SupportsUnicode();

	unique_ptr<ICharInformation> info = GetCharInformation(pos);

	for(const char32_t& letter : unistr)
	{
		info->SetPosition(pos);

		if(letter == '\n')
		{
			pos.X = Pos.X;
			pos.Y++;
			continue;
		}
		
		if(unicode)
			info->SetUnicodeChar(letter);
		else
			info->SetChar(letter);

		info->SetBackgroundColour( CColour::Blend(bgcol, info->GetBackgroundColour()) );
		info->SetForegroundColour( CColour::Blend(fgcol, info->GetForegroundColour()) );
		
		pos.X++;
	}
}

void IConsoleRenderer::PushRenderBounds(const CVector& from_a, const CSize& size_a)
{
	renderbound_t rb;

	CVector from = CVector((int)from_a.X, (int)from_a.Y);
	CSize size = CSize((int)size_a.Width, (int)size_a.Height);
	
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
	bool m_Unicode;
public:
	const static int LEFT	= (1 << 0);
	const static int RIGHT	= (1 << 1);
	const static int UP		= (1 << 2);
	const static int DOWN	= (1 << 3);
	
	CRectangleChar(bool unicode, const bool left, const bool right, const bool up, const bool down)
	{
		m_State = 0;
		m_Unicode = unicode;

		if(left)
			m_State |= LEFT;
		if(right)
			m_State |= RIGHT;
		if(up)
			m_State |= UP;
		if(down)
			m_State |= DOWN;
	}
		
	CRectangleChar(bool unicode, char32_t type)
	{
		m_Unicode = unicode;

		#define CHAR_STATE(ltr, val) \
		case ltr: \
			m_State = (val); \
			break
		if(!m_Unicode)
		{
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
		}
		else
		{
			switch(type)
			{
				CHAR_STATE(0x2574, LEFT);
				CHAR_STATE(0x2576, RIGHT);
				CHAR_STATE(0x2575, UP);
				CHAR_STATE(0x2577, DOWN);
			
				CHAR_STATE(0x2502, UP | DOWN);
				CHAR_STATE(0x2524, UP | DOWN | LEFT);
				CHAR_STATE(0x2510, DOWN | LEFT);
				CHAR_STATE(0x2514, UP | RIGHT);
				CHAR_STATE(0x2534, LEFT | UP | RIGHT);
				CHAR_STATE(0x252C, LEFT | DOWN | RIGHT);
				CHAR_STATE(0x251C, UP | DOWN | RIGHT);
				CHAR_STATE(0x2500, LEFT | RIGHT);
				CHAR_STATE(0x253C, LEFT | RIGHT | UP | DOWN);
				CHAR_STATE(0x2518, UP | LEFT);
				CHAR_STATE(0x250C, RIGHT | DOWN);

			default:
				m_State = 0;
			}
		}

		#undef CHAR_STATE
	}

	char32_t GetChar()
	{
		 // same as the one up there ^, but the only difference is it translated the other way
		#define CHAR_STATE(ltr, val) \
		case (val): \
			return (char32_t)ltr;

		if(!m_Unicode)
		{
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
		}
		else
		{
			switch(m_State)
			{
				CHAR_STATE(0x2574, LEFT);
				CHAR_STATE(0x2576, RIGHT);
				CHAR_STATE(0x2575, UP);
				CHAR_STATE(0x2577, DOWN);
			
				CHAR_STATE(0x2502, UP | DOWN);
				CHAR_STATE(0x2524, UP | DOWN | LEFT);
				CHAR_STATE(0x2510, DOWN | LEFT);
				CHAR_STATE(0x2514, UP | RIGHT);
				CHAR_STATE(0x2534, LEFT | UP | RIGHT);
				CHAR_STATE(0x252C, LEFT | DOWN | RIGHT);
				CHAR_STATE(0x251C, UP | DOWN | RIGHT);
				CHAR_STATE(0x2500, LEFT | RIGHT);
				CHAR_STATE(0x253C, LEFT | RIGHT | UP | DOWN);
				CHAR_STATE(0x2518, UP | LEFT);
				CHAR_STATE(0x250C, RIGHT | DOWN);

			default:
				m_State = 0;
			}
		}


		#undef CHAR_STATE
	}

	CRectangleChar operator+(const CRectangleChar& other)
	{
		CRectangleChar ret(m_Unicode, 0);
		ret.m_State = this->m_State | other.m_State;

		return ret;
	}

	CRectangleChar operator-(const CRectangleChar& other)
	{
		CRectangleChar ret(m_Unicode, 0);
		ret.m_State = this->m_State & ~other.m_State;

		return ret;
	}
};

void IConsoleRenderer::DrawRect(const CVector& pos, const CSize& size, const CColour& fgcol, const CColour& bgcol)
{
	bool unicode = SupportsUnicode();
	CRectangleChar Horizontal(unicode, true, true, false, false);
	CRectangleChar Vertical(unicode, false, false, true, true);

	CRectangleChar Left(unicode, true, false, false, false);
	CRectangleChar Right(unicode, false, true, false, false);

	CRectangleChar Top(unicode, false, false, true, false);
	CRectangleChar Bottom(unicode, false, false, false, true);
	
	unique_ptr<ICharInformation> info_top = GetCharInformation(CVector());
	unique_ptr<ICharInformation> info_bot = GetCharInformation(CVector());
	unique_ptr<ICharInformation> info_l = GetCharInformation(CVector());
	unique_ptr<ICharInformation> info_r = GetCharInformation(CVector());

	if(size.Width == 1)
		;
	else if(size.Height == 1)
		for(int x = pos.X; x < pos.X + size.Width; x++)
		{
			info_top->SetPosition(CVector(x, pos.Y));
			
			info_top->SetUnicodeChar((CRectangleChar(unicode, info_top->GetUnicodeChar()) + Horizontal).GetChar());
			info_top->SetForegroundColour(CColour::Blend(fgcol, info_top->GetForegroundColour()));
			info_top->SetBackgroundColour(CColour::Blend(bgcol, info_top->GetBackgroundColour()));
		}
	else
	for(int x = pos.X; x < pos.X + size.Width; x++)
	{
		info_top->SetPosition(CVector(x, pos.Y));
		info_bot->SetPosition(CVector(x, pos.Y + size.Height - 1));

		CRectangleChar cur = Horizontal;

		if(x == pos.X)
			cur = cur - Left;
		else if(x == pos.X + size.Width - 1)
			cur = cur - Right;

		info_top->SetUnicodeChar((CRectangleChar(unicode, info_top->GetUnicodeChar()) + cur).GetChar());
		info_top->SetForegroundColour(fgcol);
		info_top->SetBackgroundColour(bgcol);

		info_bot->SetUnicodeChar((CRectangleChar(unicode, info_bot->GetUnicodeChar()) + cur).GetChar());
		info_bot->SetForegroundColour(fgcol);
		info_bot->SetBackgroundColour(bgcol);
	}
	
	if(size.Height == 1)
		;
	else if(size.Width == 1)
		for(int y = pos.Y; y < pos.Y + size.Height; y++)
		{
			info_l->SetPosition(CVector(pos.X, y));

			info_l->SetUnicodeChar( (CRectangleChar(unicode, info_l->GetUnicodeChar()) + Vertical).GetChar() );
			info_l->SetForegroundColour(fgcol);
			info_l->SetBackgroundColour(bgcol);
		}
	else
	for(int y = pos.Y; y < pos.Y + size.Height; y++)
	{
		info_l->SetPosition(CVector(pos.X, y));
		info_r->SetPosition(CVector(pos.X + size.Width - 1, y));

		CRectangleChar cur = Vertical;

		if(y == pos.Y)
			cur = cur - Top;
		else if(y == pos.Y + size.Height - 1)
			cur = cur - Bottom;

		info_l->SetUnicodeChar( (CRectangleChar(unicode, info_l->GetUnicodeChar()) + cur).GetChar() );
		info_l->SetForegroundColour(fgcol);
		info_l->SetBackgroundColour(bgcol);

		info_r->SetUnicodeChar( (CRectangleChar(unicode, info_r->GetUnicodeChar()) + cur).GetChar() );
		info_r->SetForegroundColour(fgcol);
		info_r->SetBackgroundColour(bgcol);
	}
}