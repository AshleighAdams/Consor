#define WINDOWS_CONSOLERENDERER_FRIEND
#include "WindowsConsoleRenderer.hpp"

#include <sstream>
#include <assert.h>
#include <fcntl.h>
#include <io.h>
#include <locale>
#include <codecvt>

#include "Util/Debug.hpp"
#include "Util/Time.hpp"

using namespace std;

using namespace Consor;
using namespace Consor::Console;

#define DELTA_DRAW_ONLY
// the renderer sometimes bugs out, i found with some most windows console fonts it
//#define DELTA_DRAW_1_BIGGER

CColour AttributeForegroundColour(CWindowsConsoleRenderer& Renderer, CharAttributes att)
{
	int col = ((int)att >> 00) & 0x0F;
	/*
	if(att & CharAttributes::ForegroundRed)
		col += 4;
	if(att & CharAttributes::ForegroundGreen)
		col += 2;
	if(att & CharAttributes::ForegroundBlue)
		col += 1;

	if(att & CharAttributes::ForegroundIntense)
		col += 8;
	*/
	return Renderer.m_ColourTable[col];
}

CColour AttributeBackgroundColour(CWindowsConsoleRenderer& Renderer, CharAttributes att)
{
	int col = ((int)att >> 4) & 0x0F;
	/*
	if(att & CharAttributes::BackgroundRed)
		col += 4;
	if(att & CharAttributes::BackgroundGreen)
		col += 2;
	if(att & CharAttributes::BackgroundBlue)
		col += 1;

	if(att & CharAttributes::BackgroundIntense)
		col += 8;
	*/
	return Renderer.m_ColourTable[col];
}


inline bool double_equals(double a, double b)
{
	double diff = abs(a-b);
	double epsilon = 0.01; // DBL_EPSILON
	return diff < epsilon;
}

CharAttributes FromForegroundColour(CWindowsConsoleRenderer& Renderer, const CColour& targ)
{	
	int atts = 0;
	bool found = false;

	for(const CColour& col : Renderer.m_ColourTable)
	{
		if(
		  double_equals(col.R, targ.R) &&
		  double_equals(col.G, targ.G) &&
		  double_equals(col.B, targ.B) )
		{
			found = true;
			break;
		}
		atts++;
	}

	if(found)
		return (CharAttributes)atts;
	else
		return CharAttributes::None;
}

CharAttributes FromBackgroundColour(CWindowsConsoleRenderer& Renderer, const CColour& col)
{
	int atts = (int)FromForegroundColour(Renderer, col);
	return (CharAttributes)(atts << 4);
}

CWindowsConsoleRenderer::CWindowsConsoleRenderer()
{
	m_WroteOnce = false;
	m_STDOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFOEX info;
	info.cbSize = sizeof(info);

	BOOL gotinfo = GetConsoleScreenBufferInfoEx(m_STDOutHandle, &info);
	assert(gotinfo == TRUE);

	m_Width = info.srWindow.Right - info.srWindow.Left + 1;
	m_Height = info.srWindow.Bottom - info.srWindow.Top + 1;

	Util::Log("detected console size: %, %", m_Width, m_Height);

	PushRenderBounds(CVector(), CSize(m_Width, m_Height));

	m_BufferHandle = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);

	if (stdout == INVALID_HANDLE_VALUE || m_BufferHandle == INVALID_HANDLE_VALUE)
	{
		Util::Log("failed to create output buffer. version string: %", VersionString());
		throw std::exception("failed to create output buffer");
	}

	if (!SetConsoleActiveScreenBuffer(m_BufferHandle))
	{
		Util::Log("failed to activate output buffer. version string: %", VersionString());
		throw std::exception("failed to activate output buffer");
	}

	m_pBuffer = new CHAR_INFO[m_Width * m_Height];
	m_pBufferDelta = new CHAR_INFO[m_Width * m_Height];

	COORD coordBufSize = {m_Width, m_Height};
	COORD coordBufCoord = {0, 0};

	SMALL_RECT srctWriteRect;
	srctWriteRect.Top = 0;
	srctWriteRect.Left = 0;
	srctWriteRect.Right = m_Width;
	srctWriteRect.Bottom = m_Height;

	BOOL success = ReadConsoleOutputW(m_BufferHandle, m_pBuffer, coordBufSize, coordBufCoord, &srctWriteRect);

	// update the colour tables:

	CONSOLE_SCREEN_BUFFER_INFOEX info_newbuf;

	info_newbuf.cbSize = sizeof(info_newbuf);

	GetConsoleScreenBufferInfoEx(m_BufferHandle, &info_newbuf);
	
	for(size_t i = 0; i < MaxColours(); i++)
	{
		COLORREF ref = info_newbuf.ColorTable[i]; // 0x00bbggrr
		int r = (ref & 0x000000FF) >> 0;
		int g = (ref & 0x0000FF00) >> 8;
		int b = (ref & 0x00FF0000) >> 16;

		m_ColourTable[i].A = 1.0;
		m_ColourTable[i].R = (double)r / 255.0;
		m_ColourTable[i].G = (double)g / 255.0;
		m_ColourTable[i].B = (double)b / 255.0;
	}

	static_assert(sizeof(m_OriginalColourTable) == sizeof(m_ColourTable), "Colour table sizes do not match!");
	memcpy(m_OriginalColourTable, m_ColourTable, sizeof(m_OriginalColourTable));

	//hide the cursor
	CONSOLE_CURSOR_INFO cusor_info;
 
	cusor_info.dwSize = sizeof cusor_info;
	cusor_info.bVisible = false;
 
	SetConsoleCursorInfo(m_BufferHandle, &cusor_info);

	// Setup unicode
	_setmode(_fileno(stdout), _O_U16TEXT);
}

CWindowsConsoleRenderer::~CWindowsConsoleRenderer()
{
	ResetColours();

	SetConsoleActiveScreenBuffer(m_STDOutHandle);
	delete [] m_pBuffer;
	delete [] m_pBufferDelta;
}

string CWindowsConsoleRenderer::RendererName()
{
	return "WindowsRenderer";
}

string to_string()
{
	return "";
}

template<typename T>
string to_string(T x)
{
	stringstream str;
	str << x;
	return str.str();
}


string CWindowsConsoleRenderer::VersionString()
{
	return "built " __DATE__ " " __TIME__ ";"

#ifdef WINDOWS_CONSOLE_RENDERER_FAST
	" direct renderer;"
#else
	" abstract renderer;"
#endif
	" impliments and supports:"
	" 16 custom colours, unicode"
#ifdef WINDOWS_CONSOLE_RENDERER_FAST
	"; direct draw calls: DrawBox, DrawString"
	"; abstract draw calls: DrawRect"
#else
	"; direct draw calls: none"
	"; abstract draw calls: DrawBox, DrawRect, DrawString"
#endif

	"; flushmode = "
#ifdef DELTA_DRAW_ONLY
	"delta"
	#ifdef DELTA_DRAW_1_BIGGER
		" + 1"
	#endif
#else
	"all"
#endif
	"; built with: "


#if defined(__clang__)
	"Clang/LLVM " __VERSION__
#elif defined(__ICC) || defined(__INTEL_COMPILER)
	"Intel ICC/ICPC " + to_string(__INTEL_COMPILER_BUILD_DATE) + 
#elif defined(__GNUC__) || defined(__GNUG__)
	"GNU GCC/G++ " __VERSION__ 
#elif defined(__HP_cc) || defined(__HP_aCC)
	"Hewlett-Packard C/C++ " + to_string(__HP_aCC) + to_string(__HP_cc) +
#elif defined(__IBMC__) || defined(__IBMCPP__)
	"IBM XL C/C++ " __xlc__
#elif defined(_MSC_VER)
	"Microsoft Visual Studio " + to_string(_MSC_FULL_VER) +
#elif defined(__PGI)
	"Portland Group PGCC/PGCPP"
#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
	"Oracle Solaris Studio"
#else
	"unknown compiler"
#endif


	"; C++ version: " + to_string(__cplusplus) +
	"; bits: " + to_string(sizeof size_t * 8) + ";"
	;
}

void CWindowsConsoleRenderer::FlushToScreen()
{
	SMALL_RECT srctWriteRect; 

	srctWriteRect.Left = srctWriteRect.Top = 0;
	srctWriteRect.Bottom = m_Height;
	srctWriteRect.Right = m_Width;

#ifdef DELTA_DRAW_ONLY
	if(m_WroteOnce == true)
	{
		// only write the region that changed (could be the hole screen)
		CVector top_left_delta = CVector(m_Width, m_Height);
		CVector bot_right_delta = CVector(0, 0);

		for(size_t x = 0; x < m_Width; x++)
		{
			for(size_t y = 0; y < m_Height; y++)
			{
				CHAR_INFO& new_ = m_pBuffer[x + m_Width * y];
				CHAR_INFO& old = m_pBufferDelta[x + m_Width * y];

				if(new_.Attributes != old.Attributes || new_.Char.UnicodeChar != old.Char.UnicodeChar)
				{
					if(x < top_left_delta.X)
						top_left_delta.X = x;
					if(x > bot_right_delta.X)
						bot_right_delta.X = x;

					if(y < top_left_delta.Y)
						top_left_delta.Y = y;
					if(y > bot_right_delta.Y)
						bot_right_delta.Y = y;
				}
			}
		}

		if(top_left_delta == CVector(m_Width, m_Height) && bot_right_delta == CVector(0, 0))
			return;

	#ifdef DELTA_DRAW_1_BIGGER
		if(top_left_delta.X > 0)
			top_left_delta.X--;
		if(top_left_delta.Y > 0)
			top_left_delta.Y--;
		if(bot_right_delta.X < m_Width)
			bot_right_delta.X++;
		if(bot_right_delta.Y < m_Height)
			bot_right_delta.Y++;
	#endif

		srctWriteRect.Top = top_left_delta.Y;
		srctWriteRect.Left = top_left_delta.X;
		srctWriteRect.Bottom = bot_right_delta.Y;
		srctWriteRect.Right = bot_right_delta.X;

	}
#endif// end delta region

	COORD coordBufSize;
	COORD coordBufCoord;
	
	coordBufSize.X = m_Width;
	coordBufSize.Y = m_Height;
	coordBufCoord.X = 0;
	coordBufCoord.Y = 0;

	//LockWindowUpdate(GetConsoleWindow());
	BOOL success = WriteConsoleOutputW( 
		m_BufferHandle, // screen buffer to write to 
		&m_pBuffer[srctWriteRect.Left + m_Width * srctWriteRect.Top],        // buffer to copy from 
		coordBufSize,     // col-row size of chiBuffer 
		coordBufCoord,    // top left src cell in chiBuffer 
		&srctWriteRect);
	//LockWindowUpdate(0);

	if(success)
	{
		m_WroteOnce = true;
		memcpy(m_pBufferDelta, m_pBuffer, m_Width * m_Height * sizeof CHAR_INFO);
	}
}

CSize CWindowsConsoleRenderer::Size()
{
	return CSize(m_Width, m_Height);
}

size_t CWindowsConsoleRenderer::MaxColours()
{
	return sizeof(m_ColourTable) / sizeof(m_ColourTable[0]);
}

void CWindowsConsoleRenderer::GetColours(size_t Count, CColour* pColours)
{
	assert(Count <= MaxColours());
	assert(pColours);

	for(size_t i = 0; i < Count; i++)
		pColours[i] = m_ColourTable[i];
}

void CWindowsConsoleRenderer::SetColours(size_t Count, CColour* pColours)
{
	assert(Count <= MaxColours());
	assert(pColours);
	
	CONSOLE_SCREEN_BUFFER_INFOEX info;
	info.cbSize = sizeof(info);

	GetConsoleScreenBufferInfoEx(m_BufferHandle, &info);

	for(size_t i = 0; i < Count; i++)
	{
		CColour& newcol = m_ColourTable[i];
		newcol = pColours[i];
		newcol.Normalize().A = 1.0;

		int r = (int)(newcol.R * 255);
		int g = (int)(newcol.G * 255);
		int b = (int)(newcol.B * 255);

		info.ColorTable[i] = RGB(r, g, b);
	}

	//HWND console = GetConsoleWindow();
	//RECT r;
	//GetWindowRect(console, &r);

	// this fixes the window resizing bug! fuck yeah!
	info.srWindow.Top -= 1;
	info.srWindow.Right += 2;
	SetConsoleScreenBufferInfoEx(m_BufferHandle, &info); // this function has a nasty habit of resizing the bloody window...
	
	//MoveWindow(console, r.left, r.top, r.right - r.left, r.bottom - r.top, TRUE);
}

void CWindowsConsoleRenderer::ResetColours()
{
	SetColours(MaxColours(), m_OriginalColourTable);
}

void CWindowsConsoleRenderer::SetTitle(const std::string& Title)
{
	::SetConsoleTitle(Title.c_str());
}

CHAR_INFO& CWindowsConsoleRenderer::_CharInfoAt(int x, int y)
{
	static CHAR_INFO dummy; // so expected behavour is done, but it's out of the screen bounds, so let them play with a dummy value

	//if(x < 0 || x > (int)(m_CurrentRenderBound.Size.Width - 1))
	//	return dummy;
	//if(y < 0 || y > (int)(m_CurrentRenderBound.Size.Height - 1))
	//	return dummy;

	//x += m_CurrentRenderBound.Pos.X;
	//y += m_CurrentRenderBound.Pos.Y;
	x += (int)m_CurrentOffset.X;
	y += (int)m_CurrentOffset.Y;

	if(x < (int)(m_CurrentRenderBound.Pos.X))
		return dummy;
	if(y < (int)(m_CurrentRenderBound.Pos.Y))
		return dummy;

	if(x > (int)(m_CurrentRenderBound.Pos.X + m_CurrentRenderBound.Size.Width - 1))
		return dummy;
	if(y > (int)(m_CurrentRenderBound.Pos.Y + m_CurrentRenderBound.Size.Height - 1))
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

void CWindowsCharInformation::SetPosition(const CVector& pos)
{
	m_Position = pos;
}

void CWindowsCharInformation::SetAttributes(CharAttributes attr)
{
	m_pRenderer->_CharInfoAt((int)m_Position.X, (int)m_Position.Y).Attributes = attr;
}

CharAttributes CWindowsCharInformation::GetAttributes()
{
	return (CharAttributes)(int)m_pRenderer->_CharInfoAt((int)m_Position.X, (int)m_Position.Y).Attributes;
}

void CWindowsCharInformation::SetForegroundColour(const CColour& col)
{
	CColour fg = GetForegroundColour();
	CColour bg = GetBackgroundColour();

	fg = CColour::Blend(col, fg);

	CharAttributes atts = FromForegroundColour(*m_pRenderer, fg) | FromBackgroundColour(*m_pRenderer, bg);
	SetAttributes(atts);
}

CColour CWindowsCharInformation::GetForegroundColour()
{
	return AttributeForegroundColour(*m_pRenderer, GetAttributes());
}

void CWindowsCharInformation::SetBackgroundColour(const CColour& col)
{
	CColour fg = GetForegroundColour();
	CColour bg = GetBackgroundColour();

	bg = CColour::Blend(col, bg);

	CharAttributes atts = FromForegroundColour(*m_pRenderer, fg) | FromBackgroundColour(*m_pRenderer, bg);
	SetAttributes(atts);
}

CColour CWindowsCharInformation::GetBackgroundColour()
{
	return AttributeBackgroundColour(*m_pRenderer, GetAttributes());
}

void CWindowsCharInformation::SetChar(char val) 
{
	CHAR_INFO& info = m_pRenderer->_CharInfoAt((int)m_Position.X, (int)m_Position.Y);
	info.Char.UnicodeChar = 0;
	info.Char.AsciiChar = val;
}

char CWindowsCharInformation::GetChar()
{
	return m_pRenderer->_CharInfoAt((int)m_Position.X, (int)m_Position.Y).Char.AsciiChar;
}

bool CWindowsConsoleRenderer::SupportsUnicode()
{
	return true;
}

bool CWindowsCharInformation::SupportsUnicode() 
{
	return true;
}

void CWindowsCharInformation::SetUnicodeChar(char32_t val) 
{
	m_pRenderer->_CharInfoAt((int)m_Position.X, (int)m_Position.Y).Char.UnicodeChar = val;
}

char32_t CWindowsCharInformation::GetUnicodeChar()
{
	return (size_t)m_pRenderer->_CharInfoAt((int)m_Position.X, (int)m_Position.Y).Char.UnicodeChar;
}

#ifdef WINDOWS_CONSOLE_RENDERER_FAST

void CWindowsConsoleRenderer::DrawBox(const CVector& pos, const CSize& size, const CColour& col)
{
	char box = ' ';

	for(int x = pos.X; x < pos.X + size.Width; x++)
		for(int y = pos.Y; y < pos.Y + size.Height; y++)
		{
			CHAR_INFO& info = _CharInfoAt(x, y);
			info.Char.UnicodeChar = 0;
			info.Char.AsciiChar = box;

			CColour merged = CColour::Blend(col, AttributeBackgroundColour(*this, (CharAttributes)info.Attributes));
			info.Attributes = FromBackgroundColour(*this, merged);
		}
}

/*

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
			
			CColour merged_fg = CColour::Blend(fgcol, AttributeForegroundColour(*this, (CharAttributes)info_top.Attributes));
			CColour merged_bg = CColour::Blend(bgcol, AttributeBackgroundColour(*this, (CharAttributes)info_top.Attributes));
			info_top.Attributes = FromForegroundColour(*this, merged_fg) | FromBackgroundColour(*this, merged_bg);
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
		
		
		CColour merged_fg = CColour::Blend(fgcol, AttributeForegroundColour(*this, (CharAttributes)info_top.Attributes));
		CColour merged_bg = CColour::Blend(bgcol, AttributeBackgroundColour(*this, (CharAttributes)info_top.Attributes));
		info_top.Attributes = FromForegroundColour(*this, merged_fg) | FromBackgroundColour(*this, merged_bg);

		merged_fg = CColour::Blend(fgcol, AttributeForegroundColour(*this, (CharAttributes)info_bot.Attributes));
		merged_bg = CColour::Blend(bgcol, AttributeBackgroundColour(*this, (CharAttributes)info_bot.Attributes));
		info_bot.Attributes = FromForegroundColour(*this, merged_fg) | FromBackgroundColour(*this, merged_bg);
	}

	if(size.Height == 1)
		;
	else if(size.Width == 1)
		for(int y = pos.Y; y < pos.Y + size.Height; y++)
		{
			CHAR_INFO& info_l = _CharInfoAt(pos.X, y);
			info_l.Char.AsciiChar = (CRectangleChar(info_l.Char.AsciiChar) + Vertical).GetChar();
		
			CColour merged_fg = CColour::Blend(fgcol, AttributeForegroundColour(*this, (CharAttributes)info_l.Attributes));
			CColour merged_bg = CColour::Blend(bgcol, AttributeBackgroundColour(*this, (CharAttributes)info_l.Attributes));
			info_l.Attributes = FromForegroundColour(*this, merged_fg) | FromBackgroundColour(*this, merged_bg);
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

		CColour merged_fg = CColour::Blend(fgcol, AttributeForegroundColour(*this, (CharAttributes)info_l.Attributes));
		CColour merged_bg = CColour::Blend(bgcol, AttributeBackgroundColour(*this, (CharAttributes)info_l.Attributes));
		info_l.Attributes = FromForegroundColour(*this, merged_fg) | FromBackgroundColour(*this, merged_bg);

		merged_fg = CColour::Blend(fgcol, AttributeForegroundColour(*this, (CharAttributes)info_r.Attributes));
		merged_bg = CColour::Blend(bgcol, AttributeBackgroundColour(*this, (CharAttributes)info_r.Attributes));
		info_r.Attributes = FromForegroundColour(*this, merged_fg) | FromBackgroundColour(*this, merged_bg);
	}
}

*/

void CWindowsConsoleRenderer::DrawString(const std::string& str, const CVector& Pos, const CColour& fgcol, const CColour& bgcol)
{
	CVector pos = Pos;
	unique_ptr<ICharInformation> info = GetCharInformation(pos);

	u32string unistr = wstring_convert<codecvt_utf8_utf16<char32_t>, char32_t>().from_bytes(str);
	for(const char32_t& letter : unistr)
	{
		if(letter == '\n')
		{
			pos.X = Pos.X;
			pos.Y++;
			continue;
		}


		CHAR_INFO& info = _CharInfoAt(pos.X, pos.Y);
		info.Char.UnicodeChar = letter;

		int bg_col = info.Attributes & CharAttributes::BackgroundWhite;

		CColour merged_fg = CColour::Blend(fgcol, AttributeForegroundColour(*this, (CharAttributes)info.Attributes));
		CColour merged_bg = CColour::Blend(bgcol, AttributeBackgroundColour(*this, (CharAttributes)info.Attributes));
		info.Attributes = FromForegroundColour(*this, merged_fg) | FromBackgroundColour(*this, merged_bg);

		pos.X++;

		if(pos.X >= m_Width)
		{
			for(int i = 1; i <= 3; i++) // make a ... effect, as if it has been snipped off
			{
				CHAR_INFO& info = _CharInfoAt(pos.X - i, pos.Y);
				info.Char.UnicodeChar = 0;
				info.Char.AsciiChar = '.';
			}
			break;
		}
	}
}

#endif