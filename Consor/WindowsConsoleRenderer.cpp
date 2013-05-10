#include "WindowsConsoleRenderer.hpp"

#include <sstream>
#include <assert.h>

using namespace std;

using namespace Consor;
using namespace Consor::Console;

CColour AttributeForegroundColour(CharAttributes att)
{
	double R, G, B;
	R = G = B = 0;

	if(att & CharAttributes::ForegroundRed)
		R = 0.5;
	if(att & CharAttributes::ForegroundGreen)
		G = 0.5;
	if(att & CharAttributes::ForegroundBlue)
		B = 0.5;

	if(att & CharAttributes::ForegroundIntense)
	{
		R *= 2.0;
		G *= 2.0;
		B *= 2.0;
	}

	return CColour(R, G, B);
}

CColour AttributeBackgroundColour(CharAttributes att)
{
	double R, G, B;
	R = G = B = 0;

	if(att & CharAttributes::BackgroundRed)
		R = 0.5;
	if(att & CharAttributes::BackgroundGreen)
		G = 0.5;
	if(att & CharAttributes::BackgroundBlue)
		B = 0.5;

	if(att & CharAttributes::BackgroundIntense)
	{
		R *= 2.0;
		G *= 2.0;
		B *= 2.0;
	}

	return CColour(R, G, B);
}

CharAttributes FromForegroundColour(const CColour& col)
{
	CharAttributes atts = CharAttributes::None;

	if(col.R != 0)
		atts = atts | CharAttributes::ForegroundRed;
	if(col.G != 0)
		atts = atts | CharAttributes::ForegroundGreen;
	if(col.B != 0)
		atts = atts | CharAttributes::ForegroundBlue;

	if(col.R > 0.5 || col.G > 0.5 || col.B > 0.5)
		atts = atts | CharAttributes::ForegroundIntense;

	return atts;
}

CharAttributes FromBackgroundColour(const CColour& col)
{
	CharAttributes atts = CharAttributes::None;

	if(col.R != 0)
		atts = atts | CharAttributes::BackgroundRed;
	if(col.G != 0)
		atts = atts | CharAttributes::BackgroundGreen;
	if(col.B != 0)
		atts = atts | CharAttributes::BackgroundBlue;

	if(col.R > 0.5 || col.G > 0.5 || col.B > 0.5)
		atts = atts | CharAttributes::BackgroundIntense;

	return atts;
}

CWindowsConsoleRenderer::CWindowsConsoleRenderer()
{
	m_Width = 80;
	m_Height = 25;

	PushRenderBounds(CVector(), CSize(m_Width, m_Height));

	m_STDOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	m_BufferHandle = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);

	if (stdout == INVALID_HANDLE_VALUE || m_BufferHandle == INVALID_HANDLE_VALUE)
		throw std::exception("failed to create output buffer");

	if (!SetConsoleActiveScreenBuffer(m_BufferHandle)) 
		throw std::exception("failed to activate output buffer");


	m_pBuffer = new CHAR_INFO[m_Width * m_Height];

	COORD coordBufSize = {m_Width, m_Height};
	COORD coordBufCoord = {0, 0};

	SMALL_RECT srctWriteRect;
	srctWriteRect.Top = 0;
	srctWriteRect.Left = 0;
	srctWriteRect.Right = m_Width;
	srctWriteRect.Bottom = m_Height;

	bool success = ReadConsoleOutput(m_BufferHandle, m_pBuffer, coordBufSize, coordBufCoord, &srctWriteRect);
}

CWindowsConsoleRenderer::~CWindowsConsoleRenderer()
{
	SetConsoleActiveScreenBuffer(m_STDOutHandle);
	delete [] m_pBuffer;
}

void CWindowsConsoleRenderer::FlushToScreen()
{
	SMALL_RECT srctWriteRect; 

	srctWriteRect.Left = srctWriteRect.Top = 0;
	srctWriteRect.Bottom = m_Height;
	srctWriteRect.Right = m_Width;

	COORD coordBufSize;
	COORD coordBufCoord;
	
	coordBufSize.X = m_Width;
	coordBufSize.Y = m_Height;
	coordBufCoord.X = 0;
	coordBufCoord.Y = 0;

	bool success = WriteConsoleOutput( 
		m_BufferHandle, // screen buffer to write to 
		m_pBuffer,        // buffer to copy from 
		coordBufSize,     // col-row size of chiBuffer 
		coordBufCoord,    // top left src cell in chiBuffer 
		&srctWriteRect);
}

CSize CWindowsConsoleRenderer::Size()
{
	return CSize(m_Width, m_Height);
}

CHAR_INFO& CWindowsConsoleRenderer::_CharInfoAt(int x, int y)
{
	static CHAR_INFO dummy; // so expected behavour is done, but it's out of the screen bounds, so let them play with a dummy value

	if(x < 0 || x > (int)(m_CurrentRenderBound.Size.Width - 1))
		return dummy;
	if(y < 0 || y > (int)(m_CurrentRenderBound.Size.Height - 1))
		return dummy;

	//x += m_CurrentRenderBound.Pos.X;
	//y += m_CurrentRenderBound.Pos.Y;
	x += m_CurrentOffset.X;
	y += m_CurrentOffset.Y;

	if(x < (int)(m_CurrentRenderBound.Pos.X))
		return dummy;
	if(y < (int)(m_CurrentRenderBound.Pos.Y))
		return dummy;

	if(x < 0 || (int)(x >= m_Width))
		return dummy;
	if(y < 0 || (int)(y >= m_Height))
		return dummy;

	return m_pBuffer[x + m_Width * y];
}

unique_ptr<ICharInformation> CWindowsConsoleRenderer::GetCharInformation(const CVector& pos)
{
	CWindowsCharInformation* pInfo = new CWindowsCharInformation(this, pos);

	return unique_ptr<ICharInformation>(pInfo);
}

CWindowsCharInformation::CWindowsCharInformation(CWindowsConsoleRenderer* pRenderer, CVector pos)
{
	m_pRenderer = pRenderer;
	m_Position = pos;
}

void CWindowsCharInformation::SetAttributes(CharAttributes attr)
{
	m_pRenderer->_CharInfoAt(m_Position.X, m_Position.Y).Attributes = attr;
}

CharAttributes CWindowsCharInformation::GetAttributes()
{
	return (CharAttributes)(int)m_pRenderer->_CharInfoAt(m_Position.X, m_Position.Y).Attributes;
}

void CWindowsCharInformation::SetForegroundColour(const CColour& col)
{
	CColour fg = GetForegroundColour();
	CColour bg = GetBackgroundColour();

	fg = CColour::Blend(col, fg);

	CharAttributes atts = FromForegroundColour(fg) | FromBackgroundColour(bg);
	SetAttributes(atts);
}

CColour CWindowsCharInformation::GetForegroundColour()
{
	return AttributeForegroundColour(GetAttributes());
}

void CWindowsCharInformation::SetBackgroundColour(const CColour& col)
{
	CColour fg = GetForegroundColour();
	CColour bg = GetBackgroundColour();

	bg = CColour::Blend(col, bg);

	CharAttributes atts = FromForegroundColour(fg) | FromBackgroundColour(bg);
	SetAttributes(atts);
}

CColour CWindowsCharInformation::GetBackgroundColour()
{
	return AttributeBackgroundColour(GetAttributes());
}

void CWindowsCharInformation::SetChar(char val) 
{
	m_pRenderer->_CharInfoAt(m_Position.X, m_Position.Y).Char.AsciiChar = val;
}

char CWindowsCharInformation::GetChar()
{
	return m_pRenderer->_CharInfoAt(m_Position.X, m_Position.Y).Char.AsciiChar;
}

#ifdef WINDOWS_CONSOLE_RENDERER_FAST

void CWindowsConsoleRenderer::DrawBox(const CVector& pos, const CSize& size, const CColour& col)
{
	char box = ' ';

	for(int x = pos.X; x < pos.X + size.Width; x++)
		for(int y = pos.Y; y < pos.Y + size.Height; y++)
		{
			CHAR_INFO& info = _CharInfoAt(x, y);
			info.Char.AsciiChar = box;

			CColour merged = CColour::Blend(col, AttributeBackgroundColour((CharAttributes)info.Attributes));
			info.Attributes = FromBackgroundColour(merged);
		}
}


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

void CWindowsConsoleRenderer::DrawRect(const CVector& pos, const CSize& size, const CColour& fgcol, const CColour& bgcol)
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
			CHAR_INFO& info_top = _CharInfoAt(x, pos.Y);
			info_top.Char.AsciiChar = (CRectangleChar(info_top.Char.AsciiChar) + Horizontal).GetChar();
			
			CColour merged_fg = CColour::Blend(fgcol, AttributeForegroundColour((CharAttributes)info_top.Attributes));
			CColour merged_bg = CColour::Blend(bgcol, AttributeBackgroundColour((CharAttributes)info_top.Attributes));
			info_top.Attributes = FromForegroundColour(merged_fg) | FromBackgroundColour(merged_bg);
		}
	else
	for(int x = pos.X; x < pos.X + size.Width; x++)
	{
		CHAR_INFO& info_top = _CharInfoAt(x, pos.Y);
		CHAR_INFO& info_bot = _CharInfoAt(x, pos.Y + size.Height - 1);

		CRectangleChar cur = Horizontal;

		if(x == pos.X)
			cur = cur - Left;
		else if(x == pos.X + size.Width - 1)
			cur = cur - Right;

		info_top.Char.AsciiChar = (CRectangleChar(info_top.Char.AsciiChar) + cur).GetChar();
		info_bot.Char.AsciiChar = (CRectangleChar(info_bot.Char.AsciiChar) + cur).GetChar();
		
		
		CColour merged_fg = CColour::Blend(fgcol, AttributeForegroundColour((CharAttributes)info_top.Attributes));
		CColour merged_bg = CColour::Blend(bgcol, AttributeBackgroundColour((CharAttributes)info_top.Attributes));
		info_top.Attributes = FromForegroundColour(merged_fg) | FromBackgroundColour(merged_bg);

		merged_fg = CColour::Blend(fgcol, AttributeForegroundColour((CharAttributes)info_bot.Attributes));
		merged_bg = CColour::Blend(bgcol, AttributeBackgroundColour((CharAttributes)info_bot.Attributes));
		info_bot.Attributes = FromForegroundColour(merged_fg) | FromBackgroundColour(merged_bg);
	}

	if(size.Height == 1)
		;
	else if(size.Width == 1)
		for(int y = pos.Y; y < pos.Y + size.Height; y++)
		{
			CHAR_INFO& info_l = _CharInfoAt(pos.X, y);
			info_l.Char.AsciiChar = (CRectangleChar(info_l.Char.AsciiChar) + Vertical).GetChar();
		
			CColour merged_fg = CColour::Blend(fgcol, AttributeForegroundColour((CharAttributes)info_l.Attributes));
			CColour merged_bg = CColour::Blend(bgcol, AttributeBackgroundColour((CharAttributes)info_l.Attributes));
			info_l.Attributes = FromForegroundColour(merged_fg) | FromBackgroundColour(merged_bg);
		}
	else
	for(int y = pos.Y; y < pos.Y + size.Height; y++)
	{
		CHAR_INFO& info_l = _CharInfoAt(pos.X, y);
		CHAR_INFO& info_r = _CharInfoAt(pos.X + size.Width - 1, y);

		CRectangleChar cur = Vertical;

		if(y == pos.Y)
			cur = cur - Top;
		else if(y == pos.Y + size.Height - 1)
			cur = cur - Bottom;

		info_l.Char.AsciiChar = (CRectangleChar(info_l.Char.AsciiChar) + cur).GetChar();
		info_r.Char.AsciiChar = (CRectangleChar(info_r.Char.AsciiChar) + cur).GetChar();

		CColour merged_fg = CColour::Blend(fgcol, AttributeForegroundColour((CharAttributes)info_l.Attributes));
		CColour merged_bg = CColour::Blend(bgcol, AttributeBackgroundColour((CharAttributes)info_l.Attributes));
		info_l.Attributes = FromForegroundColour(merged_fg) | FromBackgroundColour(merged_bg);

		merged_fg = CColour::Blend(fgcol, AttributeForegroundColour((CharAttributes)info_r.Attributes));
		merged_bg = CColour::Blend(bgcol, AttributeBackgroundColour((CharAttributes)info_r.Attributes));
		info_r.Attributes = FromForegroundColour(merged_fg) | FromBackgroundColour(merged_bg);
	}
}

void CWindowsConsoleRenderer::DrawString(const std::string& str, const CVector& Pos, const CColour& fgcol, const CColour& bgcol)
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


		CHAR_INFO& info = _CharInfoAt(pos.X, pos.Y);
		info.Char.AsciiChar = letter;

		int bg_col = info.Attributes & CharAttributes::BackgroundWhite;

		CColour merged_fg = CColour::Blend(fgcol, AttributeForegroundColour((CharAttributes)info.Attributes));
		CColour merged_bg = CColour::Blend(bgcol, AttributeBackgroundColour((CharAttributes)info.Attributes));
		info.Attributes = FromForegroundColour(merged_fg) | FromBackgroundColour(merged_bg);

		pos.X++;

		if(pos.X >= m_Width)
		{
			for(int i = 1; i <= 3; i++) // make a ... effect, as if it has been snipped off
				_CharInfoAt(pos.X - i, pos.Y).Char.AsciiChar = '.';
			break;
		}
	}
}

#endif