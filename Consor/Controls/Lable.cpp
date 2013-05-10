#include "Lable.hpp"

using namespace Consor;

CLable::CLable()
{
	m_Text = "";
}

void CLable::SetText(std::string Text)
{
	m_Text = Text;
	m_Size = CSize(Text.length(), 1);
}

void CLable::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	CColour col = Skin.LableForeground();
	Renderer.DrawString(m_Text, CVector(), Skin.LableForeground(), CColour::None());
}