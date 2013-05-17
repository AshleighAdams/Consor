#ifndef CONTROLS_CHECKBOX_H
#define CONTROLS_CHECKBOX_H

#include "../Control.hpp"
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
		CheckBox();
		void SetText(std::string Text);
		bool Checked();
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