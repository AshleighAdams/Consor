#include "ProgressBar.hpp"
#include "Util/StringUtils.hpp"
#include "Util/Math.hpp"
#include "Units.hpp"

#include <string>

using namespace Consor;

CProgressBar::CProgressBar()
{
	m_Percent = 0;
	ForceResize(CSize(20, 1));
}

void CProgressBar::SetPercent(double Value)
{
	m_Percent = Value;
}

double CProgressBar::GetPercent()
{
	return m_Percent;
}

void CProgressBar::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	CSize size = Size();
	CColour bg = Skin.ProgressBarBackground(GetPercent());
	CColour fg = Skin.ProgressBarForeground(GetPercent());
	CVector pos;

	CSize barsz = CSize(size.Width * GetPercent(), size.Height);
	Renderer.DrawBox(pos, size, bg);
	
	char32_t block = 0x2584; //0x2588;
	if(!Renderer.SupportsUnicode())
		block = 219;

	std::unique_ptr<Console::ICharInformation> info = Renderer.GetCharInformation(CVector());

	for(int x = 0; x < barsz.Width; x++)
	{
		for(int y = 0; y < barsz.Height; y++)
		{
			info->SetPosition(CVector(x, y));
			info->SetUnicodeChar(block);
			info->SetForegroundColour(fg);
			info->SetBackgroundColour(fg);
		}
	}

	if(Renderer.SupportsUnicode() && fmod(barsz.Width, 1.0) >= 0.5)
	{
		for(int y = 0; y < barsz.Height; y++)
		{
			info->SetPosition(CVector(barsz.Width, y));
			char32_t lefthalf = 0x258c;
			info->SetUnicodeChar(lefthalf);
			info->SetForegroundColour(fg);
		}
	}
		
	double percent = Util::Round(GetPercent() * 100.0, 0.01);

	std::string percform = Util::FormatString("%", percent) + "%";
	CVector center = CVector(size.Width / 2, size.Height / 2);
	CColour tc = Skin.ProgressBarPercent(GetPercent());

	if(tc != CColour(0, 0, 0, 0))
		Renderer.DrawString(percform, center - CVector(percform.length() / 2, 0), tc, CColour::None());
}
