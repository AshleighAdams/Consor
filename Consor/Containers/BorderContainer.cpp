#include "BorderContainer.hpp"

using namespace Consor;
using namespace std;

BorderContainer::BorderContainer(CControl& Client, size_t Size)
{
	_pClient = &Client;
	_SetBorder(Size, Size, Size, Size);
}

BorderContainer::BorderContainer(CControl& Client, size_t Left, size_t Right, size_t Top, size_t Bottom)
{
	_pClient = &Client;
	_SetBorder(Left, Right, Top, Bottom);
}

void BorderContainer::_SetBorder(size_t Left, size_t Right, size_t Top, size_t Bottom)
{
	_BorderLeft = Left;
	_BorderRight = Right;
	_BorderTop = Top;
	_BorderBottom = Bottom;
}

Size BorderContainer::GetSize()
{
	Size bordersize = Size(_BorderLeft + _BorderRight, _BorderTop + _BorderBottom);
	return _pClient->GetSize() + bordersize;
}

void BorderContainer::OnResize(const Size& Size)
{
}

void BorderContainer::ForceResize(const Size& size)
{
	Size bordersize = Size(_BorderLeft + _BorderRight, _BorderTop + _BorderBottom);
	_pClient->ForceResize(size - bordersize); // subtract the border
}

void BorderContainer::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	Renderer.PushRenderBounds(Vector(_BorderLeft, _BorderTop), _pClient->GetSize());
	{
		_pClient->Draw(Renderer, HasFocus, Skin);
	}
	Renderer.PopRenderBounds();
}

bool BorderContainer::HandleInput(Input::Key Key, Input::IInputSystem& System)
{
	return _pClient->HandleInput(Key, System);
}

bool BorderContainer::CanFocus()
{
	return _pClient->CanFocus();
}