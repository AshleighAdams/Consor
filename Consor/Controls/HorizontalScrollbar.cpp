#include "HorizontalScrollbar.hpp"

using namespace Consor;

CHorizontalScrollbar::CHorizontalScrollbar(const CSize& Size)
{
	m_Value = 0;
	m_Size = Size;
}

CHorizontalScrollbar::CHorizontalScrollbar()
{
	m_Value = 0;
	m_Size = CSize();
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

	if(HasFocus)
		fgcol = Skin.ScrollForegroundFocused();

	CSize selfsize = Size();

	Renderer.DrawBox(CVector(), selfsize, bgcol);
	

	CVector pos = CVector((selfsize.Width - 1) * GetPercent(), 0);
	CSize size = CSize(1, selfsize.Height);

	if(fmod(pos.X, 1.0) > 0.5)
	{
		pos.X = (int)pos.X;

		std::unique_ptr<Console::ICharInformation> infoleft = Renderer.GetCharInformation(pos);
		std::unique_ptr<Console::ICharInformation> inforight = Renderer.GetCharInformation(pos + CVector(1, 0));

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
