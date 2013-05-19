#include "Label.hpp"
#include "../Util/StringUtils.hpp"

using namespace Consor;

Label::Label()
{
	_Text = "";
}

void Label::SetText(const std::string& Text)
{
	_Text = Text;
	_SetText = Text;
	_Size = Size(Text.length(), 1);
	OnResize(_Size);
}

void Label::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	Colour col = Skin.LabelForeground();

	if(HasFocus)
		col = Skin.LabelForegroundFocused();

	Renderer.DrawString(_Text, Vector(), col, Colour::None());
}

void Label::ForceResize(const Size& size)
{
	size_t width,height;

	_Text = Util::WrapText(_SetText, size.Width, &width, &height);
	_Size = Size(width, height); //(height > Size.Height ? height : Size.Height));

	OnResize(_Size);
}
