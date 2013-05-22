// Consor
#include "ANSIConsoleRenderer.hpp"
#include "Util/Debug.hpp"

using namespace Consor;
using namespace Consor::Console;

// STL
#include <assert.h>
#include <stdexcept>
#include <sstream>
using namespace std;

// Linux specific?
#include <sys/ioctl.h>
#include <unistd.h>
// end Linux specific ?

//Boost
#include <boost/locale.hpp>
using namespace boost::locale::conv;





ANSICharInformation::ANSICharInformation(ANSIConsoleRenderer& Parent, const Vector& pos)
{
	_pRenderer = &Parent;
	_Position = pos;
}


void ANSICharInformation::SetPosition(const Vector& pos)
{
	_Position = pos;
}

void ANSICharInformation::SetForegroundColour(const Colour& col)
{
	ANSI_CHAR_INFO& info = _pRenderer->_GetCharInfo(_Position);
	info.FG = Colour::Blend(col, info.FG);
}

Colour ANSICharInformation::GetForegroundColour()
{
	ANSI_CHAR_INFO& info = _pRenderer->_GetCharInfo(_Position);
	return info.FG;
}

void ANSICharInformation::SetBackgroundColour(const Colour& col)
{
	ANSI_CHAR_INFO& info = _pRenderer->_GetCharInfo(_Position);
	info.BG = Colour::Blend(col, info.BG);
}

Colour ANSICharInformation::GetBackgroundColour()
{
	ANSI_CHAR_INFO& info = _pRenderer->_GetCharInfo(_Position);
	return info.BG;
}

void ANSICharInformation::SetChar(char val)
{
	ANSI_CHAR_INFO& info = _pRenderer->_GetCharInfo(_Position);
	info.Letter = (char32_t)val;
}

char ANSICharInformation::GetChar()
{
	ANSI_CHAR_INFO& info = _pRenderer->_GetCharInfo(_Position);
	return (char)info.Letter;
}

bool ANSICharInformation::SupportsUnicode()
{
	return true;
}

void ANSICharInformation::SetUnicodeChar(char32_t val)
{
	ANSI_CHAR_INFO& info = _pRenderer->_GetCharInfo(_Position);
	info.Letter = val;
}

char32_t ANSICharInformation::GetUnicodeChar()
{
	ANSI_CHAR_INFO& info = _pRenderer->_GetCharInfo(_Position);
	return info.Letter;
}

ANSI_CHAR_INFO& ANSIConsoleRenderer::_GetCharInfo(const Vector& vec)
{
	static ANSI_CHAR_INFO dummy;
	
	int x = vec.X;
	int y = vec.Y;
	
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

	if(x < 0 || x >= (int)_Width)
		return dummy;
	if(y < 0 || y >= (int)_Height)
		return dummy;

	return _pBuffer[x + _Width * y];
}


ANSIConsoleRenderer::ANSIConsoleRenderer()
{
	_CurrentColour = 0;

	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	
	_Width = (size_t)w.ws_col;
	_Height = (size_t)w.ws_row;
	
	Util::Log("detected console size: % %", _Width, _Height);
	
	PushRenderBounds(Vector(), Size(_Width, _Height));
	_pBuffer = new ANSI_CHAR_INFO[_Width * _Height];
	
	for(size_t i = 0; i < _Width * _Height; i++)
	{
		_pBuffer[i].Letter = ' ';
		_pBuffer[i].FG = Colour(1,1,1);
		_pBuffer[i].BG = Colour();
	}
}

ANSIConsoleRenderer::~ANSIConsoleRenderer()
{
	delete [] _pBuffer;
	SetColours(ANSI_MAX_COLOURS, _OriginalColourTable);
	
	for(size_t i = 0; i < ANSI_MAX_COLOURS; i++)
		_ColourTable[i] = _OriginalColourTable[i] = Colour(0, 0, 0); // not implimented yet
}

// Just some helper functions for it to use
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

// platform specific stuff
string ANSIConsoleRenderer::RendererName()
{
	return "ANSIRenderer";
}

string ANSIConsoleRenderer::VersionString()
{
	return "built " __DATE__ " " __TIME__ ";";

	" abstract renderer;"
	
	" impliments and supports:"
	" " + to_string(ANSI_MAX_COLOURS) + " custom colours, unicode"
	
	"; direct draw calls: none"
	"; abstract draw calls: DrawBox, DrawRect, DrawString"
	
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
	"; bits: " + to_string(sizeof(size_t) * 8) + ";"
	;
}

tuple<Colour, bool> ANSIConsoleRenderer::_ClosestColourMatch(const Colour& target, bool only_created)
{
	size_t max = only_created ? this->_CurrentColour : this->MaxColours();

	Colour* best = &this->_ColourTable[0];
	double best_distance = Colour::Distance(*best, target);

	for(Colour& col : _NewColours) // look for any new colours, maybe this colour has been requested, but not flushed yet!
	{
		double distance = Colour::Distance(col, target);
		if(distance <= best_distance)
		{
			best_distance = distance;
			best = &col;
		}
	}

	if(_NewColours.size() != 0 && best_distance < 0.0001)
		return std::tuple<Colour, bool>(*best, true);

	if(max == 0) // none are allocated, return the first, with false (not found)
		return std::tuple<Colour, bool>(*best, false);

	for(size_t col = 1; col < max; col++)
	{
		Colour* cur = &this->_ColourTable[col];
		double distance = Colour::Distance(*cur, target);
		if(distance < best_distance)
		{
			best_distance = distance;
			best = cur;
		}
	}

	return std::tuple<Colour, bool>(*best, best_distance < 0.0001);
}

size_t ANSIConsoleRenderer::_ColourToColourIndex(const Colour& targ)
{
	size_t closest = 0;
	double closest_dist = Colour::Distance(_ColourTable[0], targ);
	
	for(size_t i = 1; i < MaxColours(); i++)
	{
		double dist = Colour::Distance(_ColourTable[i], targ);
		if(dist < closest_dist)
		{
			closest_dist = dist;
			closest = i;
		}
	}
	
	return closest;
}

void ANSIConsoleRenderer::_CheckConsoleSize()
{
}

void ANSIConsoleRenderer::FlushToScreen()
{
	if(this->_FlushColours) // write any new colours!
		this->FlushRequestedColours();

	stringstream ss;
	Colour current_fg = Colour(-1, -1, -1), current_bg = Colour(-1, -1, -1);
	
	for(size_t y = 0; y < _Height; y++)
	{
		ANSI::GenerateEscapeSequence(cout, '[', ANSI::EscapeSequence::CursorPosition, to_string(y + 1), "1"); // y; x
		
		for(size_t x = 0; x < _Width; x++)
		{
			ANSI_CHAR_INFO& info = _pBuffer[x + _Width * y];
			
			if(current_fg != info.FG)
			{
				ANSI::GenerateEscapeSequence(cout, '[', ANSI::EscapeSequence::SelectGraphicRendition, ANSI::GraphicRendition::TextColour256, "5", _ColourToColourIndex(info.FG));
				current_fg = info.FG;
			}
			
			if(current_bg != info.BG)
			{
				ANSI::GenerateEscapeSequence(cout, '[', ANSI::EscapeSequence::SelectGraphicRendition, ANSI::GraphicRendition::BackgroundColour256, "5", _ColourToColourIndex(info.BG));
				current_bg = info.BG;
			}
			
			string u8str = utf_to_utf<char>(&info.Letter, &info.Letter + 1);
			cout << u8str;
			//wcout << (wchar_t)info.Letter;
		}
	}

	if(this->_FlushColours)
		this->FlushRequestedColours();
	cout.flush();
	//cout << ss.str();
}

std::unique_ptr<ICharInformation> ANSIConsoleRenderer::GetCharInformation(const Vector& pos)
{
	ICharInformation* pInfo = new ANSICharInformation(*this, pos);
	
	std::unique_ptr<ICharInformation> info = std::unique_ptr<ICharInformation>(pInfo);
	return std::move(info);
}

Size ANSIConsoleRenderer::GetSize()
{
	return Size(_Width, _Height);
}

bool ANSIConsoleRenderer::SupportsUnicode()
{
	return true;
}

// this will probably replace existing colours
size_t ANSIConsoleRenderer::MaxColours()
{
	return ANSI_MAX_COLOURS;
}

void ANSIConsoleRenderer::GetColours(size_t Count, Colour* pColours)
{
	assert(Count <= ANSI_MAX_COLOURS);
	
	for(size_t i = 0; i < Count; i++)
		pColours[i] = _ColourTable[i];
}

string tohexcolour_byte(unsigned char col)
{
	static char letters[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
	
	unsigned char a = (col & 0xF0) >> 4;
	unsigned char b = (col & 0x0F) >> 0;
	
	string ret;
	ret += letters[a];
	ret += letters[b];
	return ret;
}

string tohexcolour(Colour col)
{
	col.Normalize();
	unsigned char r, g, b;
	r = col.R * 255;
	g = col.G * 255;
	b = col.B * 255;
	
	return "#" + tohexcolour_byte(r) + tohexcolour_byte(g) + tohexcolour_byte(b);
}

void ANSIConsoleRenderer::SetColours(size_t Count, Colour* pColours)
{
	assert(Count <= ANSI_MAX_COLOURS);
	
	for(size_t i = 0; i < Count; i++)
	{
		_ColourTable[i] = pColours[i];
		ANSI::GenerateEscapeSequence(cout, ']', '\x07', "4", to_string(i), tohexcolour(pColours[i]));
	}
}

void ANSIConsoleRenderer::ResetColours()
{
	SetColours(ANSI_MAX_COLOURS, _OriginalColourTable);
}

void ANSIConsoleRenderer::SetTitle(const string& Title)
{
	ANSI::GenerateEscapeSequence(cout, ']', '\0', "0", "\0" + Title + "\x07");
	//cout << "\x1b]0;\0" + Title + "\x07\0";
	// not implimented, but don't throw either, it really doesn't matter...
}
