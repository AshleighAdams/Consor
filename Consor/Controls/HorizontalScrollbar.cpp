#include "HorizontalScrollbar.hpp"

using namespace Consor;

CHorizontalScrollbar::CHorizontalScrollbar(const CSize& Size)
{
	m_Value = 0;
	m_Size = Size;
}

void CHorizontalScrollbar::SetPercent(double Percent)
{
	m_Value = Percent;
}

double CHorizontalScrollbar::GetPercent()
{
	return m_Value;
}

void CHorizontalScrollbar::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	CColour fgcol = Skin.ScrollForeground();
	CColour bgcol = Skin.ScrollBackground();

	CSize selfsize = Size();

	Renderer.DrawBox(CVector(), selfsize, bgcol);
	
	CVector pos = CVector(selfsize.Width * GetPercent(), 0);
	CSize size = CSize(1, selfsize.Height);

	Renderer.DrawBox(pos, size, fgcol);
}
