#ifndef CONTROLS_PASSWORDBOX_H
#define CONTROLS_PASSWORDBOX_H

#include "../Control.hpp"
#include "TextBox.hpp"
#include <string>

namespace Consor
{
	class PasswordBox : public TextBox
	{
	protected:
		char _PasswordChar;
	public:
		PasswordBox();
		virtual std::string GetText();
		virtual std::string GetPassword();
		virtual void SetPasswordChar(char Letter);
	};
}

#endif