#include "VerticalProgressBar.hpp"
#include "Util/StringUtils.hpp"
#include "Util/Math.hpp"
#include "Units.hpp"

#include <string>

using namespace Consor;

CVerticalProgressBar::CVerticalProgressBar() : CProgressBar()
{
	m_Size = CSize(1, 10);
	m_GraphMode = false;
}

void CVerticalProgressBar::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	CSize size = this->Size();
	CColour bg = Skin.ProgressBarBackground(GetPercent());
	CColour fg = Skin.ProgressBarForeground(GetPercent());

	if(HasFocus)
		fg = Skin.LabelForegroundFocused();

	CVector pos;

	CSize barsz = CSize(size.Width, size.Height * GetPercent());
	Renderer.DrawBox(pos, size, bg);
	
	char32_t block = 0x2588; //0x2588;
	if(!Renderer.SupportsUnicode())
		block = 219;

	std::unique_ptr<Console::ICharInformation> info = Renderer.GetCharInformation(CVector());

	int y;
	for(int x = 0; x < Util::Round(barsz.Width, 1.0); x++)
	{
		for(y = 0; y < Util::Round(barsz.Height, 1.0); y++)
		{
			info->SetPosition(CVector(x, size.Height - y - 1));
			info->SetUnicodeChar(block);
			info->SetForegroundColour(fg);
		}
	}

	// y++ is executed, so is still 1 higher than code above used

	if(Renderer.SupportsUnicode() && fmod(barsz.Height, 1.0) <= 0.5)
	{
		for(int x = 0; x < barsz.Width; x++)
		{
			info->SetPosition(CVector(x, size.Height - y - 1));
			char32_t bottomhalf = 0x2584;
			info->SetUnicodeChar(bottomhalf);
			info->SetForegroundColour(fg);
		}
	}
	
}

bool CVerticalProgressBar::HandleInput(Input::Key Key, Input::IInputSystem& System)
{
	if(!m_GraphMode)
		return false;

	switch(Key)
	{
	case Input::Key::Enter:
	case Input::Key::Space:
		Click(GetPercent(), m_GraphIndex);
		return true;
	default:
		return false;
	}
}

bool CVerticalProgressBar::CanFocus()
{
	return m_GraphMode;
}

void CVerticalProgressBar::SetGraphMode(size_t Index)
{
	m_GraphMode = true;
	m_GraphIndex = Index;
}
