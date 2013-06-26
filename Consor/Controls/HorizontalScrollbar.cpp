#include "HorizontalScrollbar.hpp"
#include "../Util/Math.hpp"

using namespace Consor;

HorizontalScrollbar::HorizontalScrollbar(const Size& Size)
{
	_Value = 0;
	_Size = Size;
	_ChangeSize = -1;
	_ScrollRegionSize = 1;
}

HorizontalScrollbar::HorizontalScrollbar()
{
	_Value = 0;
	_ChangeSize = -1;
	_Size = GetSize();
}

void HorizontalScrollbar::SetPercent(double Percent)
{
	_Value = Percent;
}

double HorizontalScrollbar::GetPercent()
{
	return _Value;
}

void HorizontalScrollbar::SetChangeSize(double ChangeSize)
{
	_ChangeSize = ChangeSize;
}

void HorizontalScrollbar::SetScrollRegionSize(double RegionSize)
{
	_ScrollRegionSize = RegionSize;
}

double HorizontalScrollbar::GetBarSize()
{
	return GetSize().Width / _ScrollRegionSize * GetSize().Width;
}

void HorizontalScrollbar::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
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

	Vector pos = Vector((selfsize.Width - barsize) * GetPercent(), 0);
	Size size = Size(barsize, selfsize.Height);

	//pos.Y = (int)pos.Y;

	char32_t lefthalf = 0x258C; //221;
	char32_t righthalf = 0x2590; //'▐';//222;
	char32_t full = 0x2588;

	if(!Renderer.SupportsUnicode())
	{
		lefthalf = 221;
		righthalf = 222;
		full = 219;
	}

	for(double x = pos.X; x < pos.X + size.Width; x += 0.5)
	{
		std::unique_ptr<Console::ICharInformation> info = Renderer.GetCharInformation(Vector(x, 0));
		char32_t cur = info->GetUnicodeChar();

		

		if(cur == lefthalf) // if top half already,
		{
			if(fmod(x, 1.0) >= 0.5) // and we venture into lower, set to full
				cur = full;
		}
		else if(cur == righthalf) // if bottom half already,
		{
			if(fmod(x, 1.0) <= 0.5) // and we venture into upper, set to full
				cur = full;
		}
		else // otherwise
		{
			if(fmod(x, 1.0) < 0.5) // if we're in the top half, set it
				cur = lefthalf;
			else // must be in the bottom, set that
				cur = righthalf;
		}

		info->SetUnicodeChar(cur);
		info->SetForegroundColour(fgcol);
	}

	//Renderer.DrawBox(pos, size, fgcol);

	return;

	if(fmod(pos.X, 1.0) > 0.5)
	{
		pos.X = (int)pos.X;

		std::unique_ptr<Console::ICharInformation> infoleft = Renderer.GetCharInformation(pos);
		std::unique_ptr<Console::ICharInformation> inforight = Renderer.GetCharInformation(pos + Vector(1, 0));

		infoleft->SetChar((char)221);
		inforight->SetChar((char)222);

		infoleft->SetForegroundColour(fgcol);
		inforight->SetForegroundColour(fgcol);
	}
	else
	{
		pos.X = (int)pos.X;
		std::unique_ptr<Console::ICharInformation> info = Renderer.GetCharInformation(pos);

		info->SetChar((char)219);
		info->SetForegroundColour(fgcol);
	}

	//Renderer.DrawBox(pos, size, fgcol);
}
