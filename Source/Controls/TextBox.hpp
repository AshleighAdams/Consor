#ifndef CONTROLS_TEXTBOX_H
#define CONTROLS_TEXTBOX_H

#include "../Control.hpp"
#include "../Util/Hooks.hpp"
#include <string>

namespace Consor
{
	class TextBox : public Control
	{
	protected:
		std::string _Text;
		size_t _CursorPosition;
		bool _InsertMode;
		double _LastTyped;

		virtual bool _IsTypedChar(Input::Key Key, char& letter);
	public:
		Hook<std::string> ValueChanged;
		
		TextBox();
		void SetText(const std::string& Text);
		virtual std::string GetText();
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
		virtual bool HandleInput(Input::Key Key, Input::IInputSystem& System);
		virtual bool CanFocus();
	};
}

#endif
