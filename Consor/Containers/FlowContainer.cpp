#include "FlowContainer.hpp"

#include <stdexcept>

using namespace Consor;
using namespace std;

FlowContainer::FlowContainer(FlowContainer::FlowAxis Axis, double Seperation)
{
	_Axis = Axis;
	_Seperation = Seperation;
	_Focused = 0;
}

size_t FlowContainer::_Focusable()
{
	size_t ret = 0;

	for(Control* c : _Controls)
		if(c->CanFocus())
			ret++;

	return ret;
}

Control* FlowContainer::_GetFocused()
{
	if(_Focusable() == 0)
		return nullptr;

	size_t cur = 0;

	for(Control* c : _Controls)
		if(c->CanFocus())
		{
			if(_Focused == cur)
				return c;
			cur++;
		}

	return nullptr;
}

Size FlowContainer::GetSize()
{
	Size ret;

	double span = 0.0;

	for(Control* ctrl : _Controls)
	{
		Size size = ctrl->GetSize();

		if(_Axis == FlowContainer::FlowAxis::Vertical)
		{
			if(size.Width > ret.Width)
				ret.Width = size.Width;

			span += size.Height + _Seperation;
		}
		else
		{
			if(size.Height > ret.Height)
				ret.Height = size.Height;

			span += size.Width + _Seperation;
		}
	}

	span -= _Seperation;

	if(_Axis == FlowContainer::FlowAxis::Vertical)
		ret.Height = span;
	else
		ret.Width = span;

	return ret;
}

void FlowContainer::OnResize(const Size& Size)
{
}

void FlowContainer::ForceResize(const Size& Size)
{
#ifndef __GNUG__
	throw std::exception("`FlowContainer' can't be resized, please use a `ScrollContainer'");
#endif
}

void FlowContainer::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	Vector pos = Vector(0, 0);
	Control* pFocused = _GetFocused();

	for(Control* ctrl : _Controls)
	{
		Size ctrl_size = ctrl->GetSize();

		if(Renderer.InRenderBounds(pos, ctrl_size))
		{
			Renderer.PushRenderBounds(pos, ctrl_size);
			ctrl->Draw(Renderer, HasFocus && ctrl == pFocused, Skin);
			Renderer.PopRenderBounds();
		}
		//else
		//	Renderer.SetTitle("Control out of renderbounds!");

		if(_Axis == FlowContainer::FlowAxis::Vertical)
			pos.Y += ctrl_size.Height + _Seperation;
		else
			pos.X += ctrl_size.Width + _Seperation;
	}
}

bool FlowContainer::HandleInput(Input::Key Key, Input::IInputSystem& System)
{
	Control* pFocused = _GetFocused();

	if(pFocused && pFocused->HandleInput(Key, System))
		return true;

	Input::Key Next = Input::Key::Down;
	Input::Key Prev = Input::Key::Up;

	if(_Axis == FlowAxis::Horizontal)
	{
		Next = Input::Key::Right;
		Prev = Input::Key::Left;
	}

	if(Key == Prev)
	{
		if(_Focused <= 0)
			return false;
		_Focused--;
		return true;
	}
	else if(Key == Next || Key == Input::Key::Tab)
	{
		size_t focusable = _Focusable();

		if(_Focused >= focusable - 1)
			return false;
		_Focused++;
		return true;
	}

	return false;
}

bool FlowContainer::CanFocus()
{
	for(Control* Control : _Controls) // return true if one of hour controls can obtain focus
		if(Control->CanFocus())
			return true;
	return false;
}

void FlowContainer::AddControl(Control& Control, double SizeTo)
{
	Size size = Control.GetSize();
	
	if(SizeTo  > 0)
	{
		if(_Axis == FlowAxis::Horizontal)
			size.Height = SizeTo;
		else
			size.Width = SizeTo;

		Control.ForceResize(size);
	}
	_Controls.push_back(&Control);
}
