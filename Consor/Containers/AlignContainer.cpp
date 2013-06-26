#include "AlignContainer.hpp"

using namespace Consor;
using namespace std;

AlignContainer::AlignContainer(Control& Client, AlignContainer::Axis Axis, AlignContainer::Align Align)
{
	_pClient = &Client;
	_Axis = Axis;
	_Align = Align;
	_Size = Size();
}

 Size AlignContainer::GetSize()
 {
	return _pClient->GetSize();
 }

Size AlignContainer::GetSize(Console::IConsoleRenderer& Renderer)
{
	Size ret;
	Size clientsize = _pClient->GetSize();

	Size parentsize = _Size;

	{ // stack stuff, get the parent render size, then push it back onto the stack
		Size old_size = Renderer.RenderSize();
		Vector old_pos = Renderer.RenderOffset(); // this should be fine for anything, except a manual layout

		Renderer.PopRenderBounds();
		
		parentsize = Renderer.RenderSize();

		Renderer.PushRenderBounds(old_pos, parentsize); // notice we also use a new size?
	}

	if(_Axis == AlignContainer::Axis::Horizotal)
	{
		ret.Height = clientsize.Height;
		ret.Width = parentsize.Width;
	}
	else
	{
		ret.Height = parentsize.Height;
		ret.Width = clientsize.Width;
	}
	
	if(ret.Width < clientsize.Width)
		ret.Width = clientsize.Width;
	if(ret.Height < clientsize.Height)
		ret.Height = clientsize.Height;

	return ret;
}

Vector AlignContainer::_ClientPos(Console::IConsoleRenderer& Renderer)
{
	Size size = this->GetSize(Renderer);
	Size clientsize = _pClient->GetSize();

	Vector ret;

	if(_Axis == AlignContainer::Axis::Horizotal)
	{
		switch (_Align)
		{
		case Consor::AlignContainer::Right:
			ret.X = size.Width - clientsize.Width;
			break;
		case Consor::AlignContainer::Center:
			ret.X = size.Width / 2.0 - clientsize.Width / 2.0;
			break;
		default:
			break;
		}
	}
	else
	{
		switch (_Align)
		{
		case Consor::AlignContainer::Bottom:
			ret.Y = size.Height - clientsize.Height;
			break;
		case Consor::AlignContainer::Center:
			ret.Y = size.Height / 2.0 - clientsize.Height / 2.0;
			break;
		default:
			break;
		}
	}

	return ret;
}

void AlignContainer::OnResize(const Size& Size)
{
}

void AlignContainer::ForceResize(const Size& Size)
{
	_Size = Size;
}

void AlignContainer::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	Renderer.PushRenderBounds(_ClientPos(Renderer), _pClient->GetSize());
	_pClient->Draw(Renderer, HasFocus, Skin);
	Renderer.PopRenderBounds();
}

bool AlignContainer::HandleInput(Input::Key Key, Input::IInputSystem& System)
{
	return _pClient->HandleInput(Key, System);
}

bool AlignContainer::CanFocus()
{
	return _pClient->CanFocus();
}