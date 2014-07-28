#ifndef CONTROL_H
#define CONTROL_H

#include <stdexcept>

#include "Units.hpp"
#include "Skin.hpp"
#include "ConsoleRenderer.hpp"
#include "InputSystem.hpp"

namespace Consor
{
	/// The base control.
	class Control
	{
	protected:
		Consor::Size _Size;
	public:
		virtual ~Control(){}
		/// Returns/calculates the size of the control.
		virtual Size GetSize();
		/// Called when the size of the control is changed.
		virtual void OnResize(const Size& Size);
		/// Attempts to force the control to a certain size constraint.
		virtual void ForceResize(const Size& Size);
		/// Draw itself at 0, 0, to `GetSize()`
		/// \param Renderer The renderer to draw to.
		/// \param HasFocus Whether or not the control is currently focused.
		/// \param Skin Used to select suitble colours to draw with.
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
		/// Called when the control is focused and there is input.
		/// \return `true` if we done something with the event to prevent parent controls from handeling the input (such as `ScrollContainer`s scrolling in a `TextBox`).
		virtual bool HandleInput(Input::Key Key, Input::IInputSystem& System);
		/// \return If the control is able to receive focus/or input.
		virtual bool CanFocus();
	};
};

#endif
