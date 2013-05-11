#include "VerticalScrollbar.hpp"

#include <string>
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

	if(HasFocus)
		fgcol = Skin.ScrollForegroundFocused();

	CSize selfsize = Size();

	Renderer.DrawBox(CVector(), selfsize, bgcol);
	
	CVector pos = CVector(0, (selfsize.Height - 1) * GetPercent());
	CSize size = CSize(selfsize.Width, 1);

	if(fmod(pos.Y, 1.0) > 0.5)
	{
		pos.Y = (int)pos.Y;

		std::unique_ptr<Console::ICharInformation> infotop = Renderer.GetCharInformation(pos);
		std::unique_ptr<Console::ICharInformation> infobot = Renderer.GetCharInformation(pos + CVector(0, 1));

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
