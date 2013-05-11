#ifndef CONTROLS_TEXTBOX_H
#define CONTROLS_TEXTBOX_H

#include "Control.hpp"
#include <string>

namespace Consor
{
	class CTextBox : public CControl
	{
	protected:
		std::string m_Text;
		size_t m_CursorPosition;

		virtual bool m_IsTypedChar(Input::Key Key, char& letter);
	public:
		CTextBox();
		void SetText(const std::string& Text);
		const std::string& GetText();
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
		virtual bool HandleInput(Input::Key Key);
		virtual bool CanFocus();
	};
}

#endif