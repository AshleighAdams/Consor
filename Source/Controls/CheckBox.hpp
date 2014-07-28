#ifndef CONTROLS_CHECKBOX_H
#define CONTROLS_CHECKBOX_H

#include "../Control.hpp"
#include "../Util/Hooks.hpp"
#include "Label.hpp"

#include <string>

namespace Consor
{
	class CheckBox : public Control
	{
	protected:
		Label _Label;
		bool _Checked;
	public:
		/// Fired when the value has been changed.
		Hook<bool> ValueChanged;
	
		CheckBox();
		/// Set the lable's text.
		void SetText(std::string Text);
		/// \return Whether the checkbox is checked.
		bool Checked();
		/// Set the checked state of the checkbox.
		void SetChecked(bool Value);
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
		virtual Size GetSize();
		virtual void ForceResize(const Size& Size);
		virtual void OnResize(const Size& Size);
		virtual bool HandleInput(Input::Key Key, Input::IInputSystem& System);
		virtual bool CanFocus();
	};
}

#endif
