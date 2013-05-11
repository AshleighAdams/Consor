#include "Lable.hpp"
#include "Util/StringUtils.hpp"

using namespace Consor;

CLable::CLable()
{
	m_Text = "";
}

void CLable::SetText(std::string Text)
{
	m_Text = Text;
	m_SetText = Text;
	m_Size = CSize(Text.length(), 1);
	OnResize(m_Size);
}

void CLable::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	CColour col = Skin.LableForeground();

	if(HasFocus)
		col = Skin.LableForegroundFocused();

	Renderer.DrawString(m_Text, CVector(), col, CColour::None());
}

void CLable::ForceResize(const CSize& Size)
{
	size_t width,height;

	m_Text = Util::WrapText(m_SetText, Size.Width, &width, &height);
	m_Size = CSize(width, height); //(height > Size.Height ? height : Size.Height));

	OnResize(m_Size);
}