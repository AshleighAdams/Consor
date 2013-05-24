#include "CheckBox.hpp"
#include "../Util/StringUtils.hpp"

using namespace Consor;

CheckBox::CheckBox()
{
	_Label.SetText("Default Checkbox");
	_Checked = false;
}

void CheckBox::SetText(std::string Text)
{
	_Label.SetText(Text);
}


void CheckBox::SetChecked(bool Value)
{
	_Checked = Value;
}

bool CheckBox::Checked()
{
	return _Checked;
}

void CheckBox::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	Colour col = Skin.LabelForeground();

	if(HasFocus)
		col = Skin.LabelForegroundFocused();

	Renderer.DrawString("[ ]", Vector(), col, Colour::None());

	std::unique_ptr<Console::ICharInformation> info = Renderer.GetCharInformation(Vector(1, 0));
	
	if(Checked())
	{
		//if(info->SupportsUnicode())
		//	info->SetUnicodeChar((char32_t)0x25CB);
		//else
		if(Renderer.SupportsUnicode())
			info->SetUnicodeChar(0x221A);
		else
			info->SetChar('x');
	}

	Renderer.PushRenderBounds(Vector(4, 0), _Label.GetSize());
	_Label.Draw(Renderer, false, Skin);
	Renderer.PopRenderBounds();
}

Size CheckBox::GetSize()
{
	return _Label.GetSize() + Size(4, 0);
}

void CheckBox::OnResize(const Size& size)
{
	_Label.ForceResize(size - Size(4, 0));
}

void CheckBox::ForceResize(const Size& Size)
{
	OnResize(Size);
}

bool CheckBox::HandleInput(Input::Key Key, Input::IInputSystem& System)
{
	if(Key == Input::Key::Enter || Key == Input::Key::Space)
	{
		this->SetChecked(!this->Checked());
		return true;
	}

	return false;
}

bool CheckBox::CanFocus()
{
	return true;
}
