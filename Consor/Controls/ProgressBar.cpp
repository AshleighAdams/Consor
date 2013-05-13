#include "ProgressBar.hpp"
#include "Util/StringUtils.hpp"
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
	CVector pos;

	Renderer.DrawBox(pos, size, Skin.ProgressBarBackground(GetPercent()));
	Renderer.DrawBox(pos, CSize(size.Width * GetPercent(), size.Height), Skin.ProgressBarForeground(GetPercent()));

	double percent = ((double)(int)(GetPercent() * 100.0 * 100.0)) / 100.0;

	std::string percform = Util::FormatString("%", percent) + "%";
	CVector center = CVector(size.Width / 2, size.Height / 2);
	Renderer.DrawString(percform, center - CVector(percform.length() / 2, 0), Skin.ProgressBarPercent(GetPercent()), CColour::None());
}
