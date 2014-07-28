#ifndef CONTROLS_BUTTON_H
#define CONTROLS_BUTTON_H

#include "../Control.hpp"
#include "Label.hpp"
#include "../Containers/AlignContainer.hpp"
#include "../Util/Hooks.hpp"

#include <string>

namespace Consor
{
	class Button : public Control
	{
	protected:
		AlignContainer _VAlign;
		AlignContainer _HAlign;
		Label _Label;
	public:
		/// Fired upon the button being clicked.
		Hook<> Click;

		Button();
		/// Set the button's lable's text.
		/// \param Text The string to set the lable's text to.
		void SetText(std::string Text);
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
		virtual void ForceResize(const Size& Size);
		virtual void OnResize(const Size& Size);
		virtual bool HandleInput(Input::Key Key, Input::IInputSystem& System);
		virtual bool CanFocus();
	};
}

#endif
