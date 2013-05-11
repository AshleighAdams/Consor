#include "Control.hpp"

using namespace Consor;

CSize CControl::Size()
{
	return m_Size;
}

void CControl::OnResize(const CSize& Size)
{
}

void CControl::ForceResize(const CSize& Size)
{
	m_Size = Size;
	OnResize(Size);
}

void CControl::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
}

bool CControl::HandleInput(Input::Key Key)
{
	return false;
}

bool CControl::CanFocus()
{
	return false;
}