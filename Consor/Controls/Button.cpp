#include "Button.hpp"
#include "../Util/StringUtils.hpp"

using namespace Consor;

Button::Button()
{
	_Label.SetText("Default Button");
	_HAlign = AlignContainer(_Label, _Label.GetSize(), 
		AlignContainer::Axis::Horizotal, AlignContainer::Align::Center);
	_VAlign = AlignContainer(_HAlign, _Label.GetSize(), 
		AlignContainer::Axis::Vertical, AlignContainer::Align::Center);
}

void Button::SetText(std::string Text)
{
	_Label.SetText(Text);
	ForceResize(_Label.GetSize() + Size(2, 0));
	//OnResize(_Size);
}

void Button::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	Colour col = Skin.LabelForeground();

	if(HasFocus)
		col = Skin.LabelForegroundFocused();

	Renderer.PushRenderBounds(Vector(1, 0), _VAlign.GetSize());
	_VAlign.Draw(Renderer, HasFocus, Skin);
	Renderer.PopRenderBounds();

	if(GetSize().Height == 1)
	{
		Renderer.DrawString("<", Vector(0, _Size.Height / 2.0), col, Colour::None());
		Renderer.DrawString(">", Vector(_Size.Width - 1, _Size.Height / 2.0), col, Colour::None());
	}
	else
	{
		Renderer.DrawRect(Vector(), GetSize(), col, Colour::None());
	}
}

void Button::OnResize(const Size& size)
{
	_Label.ForceResize(size - Size(2, 0));
	_HAlign.ForceResize(size - Size(2, 0));
	_VAlign.ForceResize(size - Size(2, 0));
}

void Button::ForceResize(const Size& Size)
{
	_Size = Size;
	OnResize(_Size);
}

bool Button::HandleInput(Input::Key Key, Input::IInputSystem& System)
{
	if(Key == Input::Key::Enter || Key == Input::Key::Space)
	{
		Click();
		return true;
	}

	return false;
}

bool Button::CanFocus()
{
	return true;
}
