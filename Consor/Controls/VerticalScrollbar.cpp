#include "VerticalScrollbar.hpp"

using namespace Consor;

CVerticalScrollbar::CVerticalScrollbar(const CSize& Size)
{
	m_Value = 0;
	m_Size = Size;
}

CVerticalScrollbar::CVerticalScrollbar()
{
	m_Value = 0;
	m_Size = CSize();
}

void CVerticalScrollbar::SetPercent(double Percent)
{
	m_Value = Percent;
}

double CVerticalScrollbar::GetPercent()
{
	return m_Value;
}

void CVerticalScrollbar::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	CColour fgcol = Skin.ScrollForeground();
	CColour bgcol = Skin.ScrollBackground();

	CSize selfsize = Size();

	Renderer.DrawBox(CVector(), selfsize, bgcol);
	
	CVector pos = CVector(0, (int)((selfsize.Height - 1) * GetPercent()));
	CSize size = CSize(selfsize.Width, 1);

	Renderer.DrawBox(pos, size, fgcol);
}
