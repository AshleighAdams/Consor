#include "Control.hpp"

using namespace Consor;

Size Control::GetSize()
{
	return _Size;
}

void Control::OnResize(const Size& Size)
{
}

void Control::ForceResize(const Size& Size)
{
	_Size = Size;
	OnResize(Size);
}

void Control::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
}

bool Control::HandleInput(Input::Key Key, Input::IInputSystem& System)
{
	return false;
}

bool Control::CanFocus()
{
	return false;
}
