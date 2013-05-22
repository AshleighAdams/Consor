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

Colour AttributeForegroundColour(WindowsConsoleRenderer& Renderer, CharAttributes att)
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
	return Renderer._ColourTable[col];
}

Colour AttributeBackgroundColour(WindowsConsoleRenderer& Renderer, CharAttributes att)
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
	return Renderer._ColourTable[col];
}


inline bool double_equals(double a, double b)
{
	double diff = abs(a-b);
	double epsilon = 0.01; // DBL_EPSILON
	return diff < epsilon;
}

CharAttributes FromForegroundColour(WindowsConsoleRenderer& Renderer, const Colour& targ)
{	
	int atts = 0;
	bool found = false;

	for(const Colour& col : Renderer._ColourTable)
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

CharAttributes FromBackgroundColour(WindowsConsoleRenderer& Renderer, const Colour& col)
{
	int atts = (int)FromForegroundColour(Renderer, col);
	return (CharAttributes)(atts << 4);
}

WindowsConsoleRenderer::WindowsConsoleRenderer()
{
	_WroteOnce = false;
	_STDOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFOEX info;
	info.cbSize = sizeof(info);

	BOOL gotinfo = GetConsoleScreenBufferInfoEx(_STDOutHandle, &info);
	assert(gotinfo == TRUE);

	_Width = info.srWindow.Right - info.srWindow.Left + 1;
	_Height = info.srWindow.Bottom - info.srWindow.Top + 1;

	Util::Log("detected console size: %, %", _Width, _Height);

	PushRenderBounds(Vector(), Size(_Width, _Height));

	_BufferHandle = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);

	if (stdout == INVALID_HANDLE_VALUE || _BufferHandle == INVALID_HANDLE_VALUE)
	{
		Util::Log("failed to create output buffer. version string: %", VersionString());
		throw std::exception("failed to create output buffer");
	}

	if (!SetConsoleActiveScreenBuffer(_BufferHandle))
	{
		Util::Log("failed to activate output buffer. version string: %", VersionString());
		throw std::exception("failed to activate output buffer");
	}

	_pBuffer = new CHAR_INFO[_Width * _Height];
	_pBufferDelta = new CHAR_INFO[_Width * _Height];

	COORD coordBufSize = {_Width, _Height};
	COORD coordBufCoord = {0, 0};

	SMALL_RECT srctWriteRect;
	srctWriteRect.Top = 0;
	srctWriteRect.Left = 0;
	srctWriteRect.Right = _Width;
	srctWriteRect.Bottom = _Height;

	BOOL success = ReadConsoleOutputW(_BufferHandle, _pBuffer, coordBufSize, coordBufCoord, &srctWriteRect);

	// update the colour tables:

	CONSOLE_SCREEN_BUFFER_INFOEX info_newbuf;

	info_newbuf.cbSize = sizeof(info_newbuf);

	GetConsoleScreenBufferInfoEx(_BufferHandle, &info_newbuf);
	
	for(size_t i = 0; i < MaxColours(); i++)
	{
		COLORREF ref = info_newbuf.ColorTable[i]; // 0x00bbggrr
		int r = (ref & 0x000000FF) >> 0;
		int g = (ref & 0x0000FF00) >> 8;
		int b = (ref & 0x00FF0000) >> 16;

		_ColourTable[i].A = 1.0;
		_ColourTable[i].R = (double)r / 255.0;
		_ColourTable[i].G = (double)g / 255.0;
		_ColourTable[i].B = (double)b / 255.0;
	}

	static_assert(sizeof(_OriginalColourTable) == sizeof(_ColourTable), "Colour table sizes do not match!");
	memcpy(_OriginalColourTable, _ColourTable, sizeof(_OriginalColourTable));

	//hide the cursor
	CONSOLE_CURSOR_INFO cusor_info;
 
	cusor_info.dwSize = sizeof cusor_info;
	cusor_info.bVisible = false;
 
	SetConsoleCursorInfo(_BufferHandle, &cusor_info);

	// Setup unicode
	_setmode(_fileno(stdout), _O_U16TEXT);
}

WindowsConsoleRenderer::~WindowsConsoleRenderer()
{
	ResetColours();

	SetConsoleActiveScreenBuffer(_STDOutHandle);
	delete [] _pBuffer;
	delete [] _pBufferDelta;
}

string WindowsConsoleRenderer::RendererName()
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


string WindowsConsoleRenderer::VersionString()
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

void WindowsConsoleRenderer::FlushToScreen()
{
	SMALL_RECT srctWriteRect; 

	srctWriteRect.Left = srctWriteRect.Top = 0;
	srctWriteRect.Bottom = _Height;
	srctWriteRect.Right = _Width;

#ifdef DELTA_DRAW_ONLY
	if(_WroteOnce == true)
	{
		// only write the region that changed (could be the hole screen)
		Vector top_left_delta = Vector(_Width, _Height);
		Vector bot_right_delta = Vector(0, 0);

		for(size_t x = 0; x < _Width; x++)
		{
			for(size_t y = 0; y < _Height; y++)
			{
				CHAR_INFO& new_ = _pBuffer[x + _Width * y];
				CHAR_INFO& old = _pBufferDelta[x + _Width * y];

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

		if(top_left_delta == Vector(_Width, _Height) && bot_right_delta == Vector(0, 0))
			return;

	#ifdef DELTA_DRAW_1_BIGGER
		if(top_left_delta.X > 0)
			top_left_delta.X--;
		if(top_left_delta.Y > 0)
			top_left_delta.Y--;
		if(bot_right_delta.X < _Width)
			bot_right_delta.X++;
		if(bot_right_delta.Y < _Height)
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
	
	coordBufSize.X = _Width;
	coordBufSize.Y = _Height;
	coordBufCoord.X = 0;
	coordBufCoord.Y = 0;

	//LockWindowUpdate(GetConsoleWindow());
	BOOL success = WriteConsoleOutputW( 
		_BufferHandle, // screen buffer to write to 
		&_pBuffer[srctWriteRect.Left + _Width * srctWriteRect.Top],        // buffer to copy from 
		coordBufSize,     // col-row size of chiBuffer 
		coordBufCoord,    // top left src cell in chiBuffer 
		&srctWriteRect);
	//LockWindowUpdate(0);

	if(success)
	{
		_WroteOnce = true;
		memcpy(_pBufferDelta, _pBuffer, _Width * _Height * sizeof CHAR_INFO);
	}
}

Size WindowsConsoleRenderer::GetSize()
{
	return Size(_Width, _Height);
}

size_t WindowsConsoleRenderer::MaxColours()
{
	return sizeof(_ColourTable) / sizeof(_ColourTable[0]);
}

void WindowsConsoleRenderer::GetColours(size_t Count, Colour* pColours)
{
	assert(Count <= MaxColours());
	assert(pColours);

	for(size_t i = 0; i < Count; i++)
		pColours[i] = _ColourTable[i];
}

void WindowsConsoleRenderer::SetColours(size_t Count, Colour* pColours)
{
	assert(Count <= MaxColours());
	assert(pColours);
	
	CONSOLE_SCREEN_BUFFER_INFOEX info;
	info.cbSize = sizeof(info);

	GetConsoleScreenBufferInfoEx(_BufferHandle, &info);

	for(size_t i = 0; i < Count; i++)
	{
		Colour& newcol = _ColourTable[i];
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
	SetConsoleScreenBufferInfoEx(_BufferHandle, &info); // this function has a nasty habit of resizing the bloody window...
	
	//MoveWindow(console, r.left, r.top, r.right - r.left, r.bottom - r.top, TRUE);
}

void WindowsConsoleRenderer::ResetColours()
{
	SetColours(MaxColours(), _OriginalColourTable);
}

void WindowsConsoleRenderer::SetTitle(const std::string& Title)
{
	::SetConsoleTitle(Title.c_str());
}

CHAR_INFO& WindowsConsoleRenderer::_CharInfoAt(int x, int y)
{
	static CHAR_INFO dummy; // so expected behavour is done, but it's out of the screen bounds, so let them play with a dummy value

	//if(x < 0 || x > (int)(_CurrentRenderBound.Size.Width - 1))
	//	return dummy;
	//if(y < 0 || y > (int)(_CurrentRenderBound.Size.Height - 1))
	//	return dummy;

	//x += _CurrentRenderBound.Pos.X;
	//y += _CurrentRenderBound.Pos.Y;
	x += (int)_CurrentOffset.X;
	y += (int)_CurrentOffset.Y;

	if(x < (int)(_CurrentRenderBound.pos.X))
		return dummy;
	if(y < (int)(_CurrentRenderBound.pos.Y))
		return dummy;

	if(x > (int)(_CurrentRenderBound.pos.X + _CurrentRenderBound.size.Width - 1))
		return dummy;
	if(y > (int)(_CurrentRenderBound.pos.Y + _CurrentRenderBound.size.Height - 1))
		return dummy;

	if(x < 0 || (int)(x >= _Width))
		return dummy;
	if(y < 0 || (int)(y >= _Height))
		return dummy;

	return _pBuffer[x + _Width * y];
}

unique_ptr<ICharInformation> WindowsConsoleRenderer::GetCharInformation(const Vector& pos)
{
	CWindowsCharInformation* pInfo = new CWindowsCharInformation(this, pos);

	return unique_ptr<ICharInformation>(pInfo);
}

CWindowsCharInformation::CWindowsCharInformation(WindowsConsoleRenderer* pRenderer, Vector pos)
{
	_pRenderer = pRenderer;
	_Position = pos;
}

void CWindowsCharInformation::SetPosition(const Vector& pos)
{
	_Position = pos;
}

void CWindowsCharInformation::SetAttributes(CharAttributes attr)
{
	_pRenderer->_CharInfoAt((int)_Position.X, (int)_Position.Y).Attributes = attr;
}

CharAttributes CWindowsCharInformation::GetAttributes()
{
	return (CharAttributes)(int)_pRenderer->_CharInfoAt((int)_Position.X, (int)_Position.Y).Attributes;
}

void CWindowsCharInformation::SetForegroundColour(const Colour& col)
{
	Colour fg = GetForegroundColour();
	Colour bg = GetBackgroundColour();

	fg = Colour::Blend(col, fg);

	CharAttributes atts = FromForegroundColour(*_pRenderer, fg) | FromBackgroundColour(*_pRenderer, bg);
	SetAttributes(atts);
}

Colour CWindowsCharInformation::GetForegroundColour()
{
	return AttributeForegroundColour(*_pRenderer, GetAttributes());
}

void CWindowsCharInformation::SetBackgroundColour(const Colour& col)
{
	Colour fg = GetForegroundColour();
	Colour bg = GetBackgroundColour();

	bg = Colour::Blend(col, bg);

	CharAttributes atts = FromForegroundColour(*_pRenderer, fg) | FromBackgroundColour(*_pRenderer, bg);
	SetAttributes(atts);
}

Colour CWindowsCharInformation::GetBackgroundColour()
{
	return AttributeBackgroundColour(*_pRenderer, GetAttributes());
}

void CWindowsCharInformation::SetChar(char val) 
{
	CHAR_INFO& info = _pRenderer->_CharInfoAt((int)_Position.X, (int)_Position.Y);
	info.Char.UnicodeChar = 0;
	info.Char.AsciiChar = val;
}

char CWindowsCharInformation::GetChar()
{
	return _pRenderer->_CharInfoAt((int)_Position.X, (int)_Position.Y).Char.AsciiChar;
}

bool WindowsConsoleRenderer::SupportsUnicode()
{
	return true;
}

bool CWindowsCharInformation::SupportsUnicode() 
{
	return true;
}

void CWindowsCharInformation::SetUnicodeChar(char32_t val) 
{
	_pRenderer->_CharInfoAt((int)_Position.X, (int)_Position.Y).Char.UnicodeChar = val;
}

char32_t CWindowsCharInformation::GetUnicodeChar()
{
	return (size_t)_pRenderer->_CharInfoAt((int)_Position.X, (int)_Position.Y).Char.UnicodeChar;
}

#ifdef WINDOWS_CONSOLE_RENDERER_FAST

void WindowsConsoleRenderer::DrawBox(const Vector& pos, const Size& size, const Colour& col)
{
	char box = ' ';

	for(int x = pos.X; x < pos.X + size.Width; x++)
		for(int y = pos.Y; y < pos.Y + size.Height; y++)
		{
			CHAR_INFO& info = _CharInfoAt(x, y);
			info.Char.UnicodeChar = 0;
			info.Char.AsciiChar = box;

			Colour merged = Colour::Blend(col, AttributeBackgroundColour(*this, (CharAttributes)info.Attributes));
			info.Attributes = FromBackgroundColour(*this, merged);
		}
}

/*

class RectangleChar
{
	int _State;
public:
	const static int LEFT	= (1 << 0);
	const static int RIGHT	= (1 << 1);
	const static int UP		= (1 << 2);
	const static int DOWN	= (1 << 3);
	
	RectangleChar(const bool left, const bool right, const bool up, const bool down)
	{
		_State = 0;

		if(left)
			_State |= LEFT;
		if(right)
			_State |= RIGHT;
		if(up)
			_State |= UP;
		if(down)
			_State |= DOWN;
	}
		
	RectangleChar(unsigned char type)
	{
		#define CHAR_STATE(ltr, val) \
		case ltr: \
			_State = (val); \
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
			_State = 0;
		}

		#undef CHAR_STATE
	}

	char GetChar()
	{
		 // same as the one up there ^, but the only difference is it translated the other way
		#define CHAR_STATE(ltr, val) \
		case (val): \
			return (char)ltr;

		switch(_State)
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

	RectangleChar operator+(const RectangleChar& other)
	{
		RectangleChar ret(0);
		ret._State = this->_State | other._State;

		return ret;
	}

	RectangleChar operator-(const RectangleChar& other)
	{
		RectangleChar ret(0);
		ret._State = this->_State & ~other._State;

		return ret;
	}
};

void WindowsConsoleRenderer::DrawRect(const Vector& pos, const Size& size, const Colour& fgcol, const Colour& bgcol)
{
	RectangleChar Horizontal(true, true, false, false);
	RectangleChar Vertical(false, false, true, true);

	RectangleChar Left(true, false, false, false);
	RectangleChar Right(false, true, false, false);

	RectangleChar Top(false, false, true, false);
	RectangleChar Bottom(false, false, false, true);
	
	if(size.Width == 1)
		;
	else if(size.Height == 1)
		for(int x = pos.X; x < pos.X + size.Width; x++)
		{
			CHAR_INFO& info_top = _CharInfoAt(x, pos.Y);
			info_top.Char.AsciiChar = (RectangleChar(info_top.Char.AsciiChar) + Horizontal).GetChar();
			
			Colour merged_fg = Colour::Blend(fgcol, AttributeForegroundColour(*this, (CharAttributes)info_top.Attributes));
			Colour merged_bg = Colour::Blend(bgcol, AttributeBackgroundColour(*this, (CharAttributes)info_top.Attributes));
			info_top.Attributes = FromForegroundColour(*this, merged_fg) | FromBackgroundColour(*this, merged_bg);
		}
	else
	for(int x = pos.X; x < pos.X + size.Width; x++)
	{
		CHAR_INFO& info_top = _CharInfoAt(x, pos.Y);
		CHAR_INFO& info_bot = _CharInfoAt(x, pos.Y + size.Height - 1);

		RectangleChar cur = Horizontal;

		if(x == pos.X)
			cur = cur - Left;
		else if(x == pos.X + size.Width - 1)
			cur = cur - Right;

		info_top.Char.AsciiChar = (RectangleChar(info_top.Char.AsciiChar) + cur).GetChar();
		info_bot.Char.AsciiChar = (RectangleChar(info_bot.Char.AsciiChar) + cur).GetChar();
		
		
		Colour merged_fg = Colour::Blend(fgcol, AttributeForegroundColour(*this, (CharAttributes)info_top.Attributes));
		Colour merged_bg = Colour::Blend(bgcol, AttributeBackgroundColour(*this, (CharAttributes)info_top.Attributes));
		info_top.Attributes = FromForegroundColour(*this, merged_fg) | FromBackgroundColour(*this, merged_bg);

		merged_fg = Colour::Blend(fgcol, AttributeForegroundColour(*this, (CharAttributes)info_bot.Attributes));
		merged_bg = Colour::Blend(bgcol, AttributeBackgroundColour(*this, (CharAttributes)info_bot.Attributes));
		info_bot.Attributes = FromForegroundColour(*this, merged_fg) | FromBackgroundColour(*this, merged_bg);
	}

	if(size.Height == 1)
		;
	else if(size.Width == 1)
		for(int y = pos.Y; y < pos.Y + size.Height; y++)
		{
			CHAR_INFO& info_l = _CharInfoAt(pos.X, y);
			info_l.Char.AsciiChar = (RectangleChar(info_l.Char.AsciiChar) + Vertical).GetChar();
		
			Colour merged_fg = Colour::Blend(fgcol, AttributeForegroundColour(*this, (CharAttributes)info_l.Attributes));
			Colour merged_bg = Colour::Blend(bgcol, AttributeBackgroundColour(*this, (CharAttributes)info_l.Attributes));
			info_l.Attributes = FromForegroundColour(*this, merged_fg) | FromBackgroundColour(*this, merged_bg);
		}
	else
	for(int y = pos.Y; y < pos.Y + size.Height; y++)
	{
		CHAR_INFO& info_l = _CharInfoAt(pos.X, y);
		CHAR_INFO& info_r = _CharInfoAt(pos.X + size.Width - 1, y);

		RectangleChar cur = Vertical;

		if(y == pos.Y)
			cur = cur - Top;
		else if(y == pos.Y + size.Height - 1)
			cur = cur - Bottom;

		info_l.Char.AsciiChar = (RectangleChar(info_l.Char.AsciiChar) + cur).GetChar();
		info_r.Char.AsciiChar = (RectangleChar(info_r.Char.AsciiChar) + cur).GetChar();

		Colour merged_fg = Colour::Blend(fgcol, AttributeForegroundColour(*this, (CharAttributes)info_l.Attributes));
		Colour merged_bg = Colour::Blend(bgcol, AttributeBackgroundColour(*this, (CharAttributes)info_l.Attributes));
		info_l.Attributes = FromForegroundColour(*this, merged_fg) | FromBackgroundColour(*this, merged_bg);

		merged_fg = Colour::Blend(fgcol, AttributeForegroundColour(*this, (CharAttributes)info_r.Attributes));
		merged_bg = Colour::Blend(bgcol, AttributeBackgroundColour(*this, (CharAttributes)info_r.Attributes));
		info_r.Attributes = FromForegroundColour(*this, merged_fg) | FromBackgroundColour(*this, merged_bg);
	}
}

*/

void WindowsConsoleRenderer::DrawString(const std::string& str, const Vector& Pos, const Colour& fgcol, const Colour& bgcol)
{
	Vector pos = Pos;
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

		Colour merged_fg = Colour::Blend(fgcol, AttributeForegroundColour(*this, (CharAttributes)info.Attributes));
		Colour merged_bg = Colour::Blend(bgcol, AttributeBackgroundColour(*this, (CharAttributes)info.Attributes));
		info.Attributes = FromForegroundColour(*this, merged_fg) | FromBackgroundColour(*this, merged_bg);

		pos.X++;

		if(pos.X >= _Width)
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