#include "VerticalScrollbar.hpp"

#include "../Util/Math.hpp"

#include <string>
#include <cmath>
using namespace Consor;

VerticalScrollbar::VerticalScrollbar(const Size& Size)
{
	_Value = 0;
	_Size = Size;
	_ChangeSize = -1;
}

VerticalScrollbar::VerticalScrollbar()
{
	_Value = 0;
	_ChangeSize = -1;
	_Size = GetSize();
}

void VerticalScrollbar::SetPercent(double Percent)
{
	_Value = Percent;
}

double VerticalScrollbar::GetPercent()
{
	return _Value;
}

void VerticalScrollbar::SetChangeSize(double ChangeSize)
{
	_ChangeSize = ChangeSize;
}

double VerticalScrollbar::GetBarSize()
{
	if(_ChangeSize <= 0)
		return 1;
	return _ChangeSize * GetSize().Height;
}


void VerticalScrollbar::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	Colour fgcol = Skin.ScrollForeground();
	Colour bgcol = Skin.ScrollBackground();

	if(HasFocus)
		fgcol = Skin.ScrollForegroundFocused();

	Size selfsize = this->GetSize();

	Renderer.DrawBox(Vector(), selfsize, bgcol);
	
	double barsize =  Util::Round(GetBarSize(), 0.5);
	if(barsize < 0.5)
		barsize = 0.5;

	Vector pos = Vector(0, (selfsize.Height - barsize) * GetPercent());
	Size size = Size(selfsize.Width, barsize);

	//pos.Y = (int)pos.Y;

	char32_t tophalf = 0x2580;
	char32_t bottomhalf = 0x2584;
	char32_t full = 0x2588;

	if(!Renderer.SupportsUnicode())
	{
		tophalf = 223;
		bottomhalf = 220;
		full = 219;
	}

	for(double y = pos.Y; y < pos.Y + size.Height; y += 0.5)
	{
		std::unique_ptr<Console::ICharInformation> info = Renderer.GetCharInformation(Vector(0, y));
		char32_t cur = info->GetUnicodeChar();

		if(cur == tophalf) // if top half already,
		{
			if(fmod(y, 1.0) >= 0.5) // and we venture into lower, set to full
				cur = full;
		}
		else if(cur == bottomhalf) // if bottom half already,
		{
			if(fmod(y, 1.0) <= 0.5) // and we venture into upper, set to full
				cur = full;
		}
		else // otherwise
		{
			if(fmod(y, 1.0) < 0.5) // if we're in the top half, set it
				cur = tophalf;
			else // must be in the bottom, set that
				cur = bottomhalf;
		}

		info->SetUnicodeChar(cur);
		info->SetForegroundColour(fgcol);
	}

	//Renderer.DrawBox(pos, size, fgcol);

	return;

	if(fmod(pos.Y, 1.0) > 0.5)
	{
		pos.Y = (int)pos.Y;

		std::unique_ptr<Console::ICharInformation> infotop = Renderer.GetCharInformation(pos);
		std::unique_ptr<Console::ICharInformation> infobot = Renderer.GetCharInformation(pos + Vector(0, 1));

		infotop->SetChar((char)220);
		infobot->SetChar((char)223);

		infotop->SetForegroundColour(fgcol);
		infobot->SetForegroundColour(fgcol);
	}
	else
	{
		pos.Y = (int)pos.Y;
		std::unique_ptr<Console::ICharInformation> info = Renderer.GetCharInformation(pos);

		info->SetChar((char)219);
		info->SetForegroundColour(fgcol);
		//Renderer.DrawBox(pos, size, fgcol);
	}
}
