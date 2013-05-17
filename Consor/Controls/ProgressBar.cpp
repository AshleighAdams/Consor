#include "ProgressBar.hpp"
#include "Util/StringUtils.hpp"
#include "Util/Math.hpp"
#include "Units.hpp"

#include <string>

using namespace Consor;

CProgressBar::CProgressBar()
{
	_Percent = 0;
	ForceResize(Size(20, 1));
}

void CProgressBar::SetPercent(double Value)
{
	_Percent = Value;
}

double CProgressBar::GetPercent()
{
	return _Percent;
}

void CProgressBar::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	Size size = this->GetSize();
	Colour bg = Skin.ProgressBarBackground(GetPercent());
	Colour fg = Skin.ProgressBarForeground(GetPercent());
	Vector pos;

	Size barsz = Size(size.Width * GetPercent(), size.Height);
	Renderer.DrawBox(pos, size, bg);
	
	char32_t block = 0x2588; //0x2588;
	if(!Renderer.SupportsUnicode())
		block = 219;

	std::unique_ptr<Console::ICharInformation> info = Renderer.GetCharInformation(Vector());

	for(int x = 0; x < barsz.Width; x++)
	{
		for(int y = 0; y < barsz.Height; y++)
		{
			info->SetPosition(Vector(x, y));
			info->SetUnicodeChar(block);
			info->SetForegroundColour(fg);
			info->SetBackgroundColour(fg);
		}
	}

	if(Renderer.SupportsUnicode() && fmod(barsz.Width, 1.0) >= 0.5)
	{
		for(int y = 0; y < barsz.Height; y++)
		{
			info->SetPosition(Vector(barsz.Width, y));
			char32_t lefthalf = 0x258c;
			info->SetUnicodeChar(lefthalf);
			info->SetForegroundColour(fg);
		}
	}
		
	double percent = Util::Round(GetPercent() * 100.0, 0.01);

	std::string percform = Util::FormatString("%", percent) + "%";
	Vector center = Vector(size.Width / 2, size.Height / 2);
	Colour tc = Skin.ProgressBarPercent(GetPercent());

	if(tc != Colour(0, 0, 0, 0))
		Renderer.DrawString(percform, center - Vector(percform.length() / 2, 0), tc, Colour::None());
}
