
#include "ANSIConsoleRenderer.hpp"

using namespace Consor;
using namespace Consor::Console;

//ANSICharInformation

/*
	ANSIConsoleRenderer* _pRenderer;
	Vector _Position;
	Colour _ColourTable[ANSI_MAX_COLOURS];
*/

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
	static ANSI_CHAR_INFO null;
	
	Vector realpos = Vector((int)vec.X, (int)vec.Y);
	
	if(!InRenderBounds(realpos, nullptr))
		return null;
		
	return _pBuffer[(int)realpos.X + _Height * (int)realpos.Y];
}

