#include "Control.hpp"

using namespace Consor;

Size CControl::GetSize()
{
	return _Size;
}

void CControl::OnResize(const Size& Size)
{
}

void CControl::ForceResize(const Size& Size)
{
	_Size = Size;
	OnResize(Size);
}

void CControl::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
}

bool CControl::HandleInput(Input::Key Key, Input::IInputSystem& System)
{
	return false;
}

bool CControl::CanFocus()
{
	return false;
}
