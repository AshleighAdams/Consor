#include "VerticalProgressBar.hpp"
#include "../Util/StringUtils.hpp"
#include "../Util/Math.hpp"
#include "../Units.hpp"

#include <string>

using namespace Consor;

VerticalProgressBar::VerticalProgressBar() : ProgressBar()
{
	_Size = Size(1, 10);
	_GraphMode = false;
}

void VerticalProgressBar::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	Size size = this->GetSize();
	Colour bg = Skin.ProgressBarBackground(GetPercent());
	Colour fg = Skin.ProgressBarForeground(GetPercent());

	if(HasFocus)
		fg = Skin.LabelForegroundFocused();

	Vector pos;

	Size barsz = Size(size.Width, size.Height * GetPercent());
	Renderer.DrawBox(pos, size, bg);
	
	char32_t block = 0x2588; //0x2588;
	if(!Renderer.SupportsUnicode())
		block = 219;

	std::unique_ptr<Console::ICharInformation> info = Renderer.GetCharInformation(Vector());

	int y = 0;
	for(int x = 0; x < Util::Round(barsz.Width, 1.0); x++)
	{
		for(y = 0; y < Util::Round(barsz.Height, 1.0); y++)
		{
			info->SetPosition(Vector(x, size.Height - y - 1));
			info->SetUnicodeChar(block);
			info->SetForegroundColour(fg);
		}
	}

	// y++ is executed, so is still 1 higher than code above used

	if(Renderer.SupportsUnicode() && fmod(barsz.Height, 1.0) <= 0.5)
	{
		for(int x = 0; x < barsz.Width; x++)
		{
			info->SetPosition(Vector(x, size.Height - y - 1));
			char32_t bottomhalf = 0x2584;
			info->SetUnicodeChar(bottomhalf);
			info->SetForegroundColour(fg);
		}
	}
	
}

bool VerticalProgressBar::HandleInput(Input::Key Key, Input::IInputSystem& System)
{
	if(!_GraphMode)
		return false;

	switch(Key)
	{
	case Input::Key::Enter:
	case Input::Key::Space:
		Click(GetPercent(), _GraphIndex);
		return true;
	default:
		return false;
	}
}

bool VerticalProgressBar::CanFocus()
{
	return _GraphMode;
}

void VerticalProgressBar::SetGraphMode(size_t Index)
{
	_GraphMode = true;
	_GraphIndex = Index;
}
