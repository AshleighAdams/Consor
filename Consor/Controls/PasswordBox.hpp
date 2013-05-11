#ifndef CONTROLS_PASSWORDBOX_H
#define CONTROLS_PASSWORDBOX_H

#include "Control.hpp"
#include "TextBox.hpp"
#include <string>

namespace Consor
{
	class CPasswordBox : public CTextBox
	{
	protected:
		char m_PasswordChar;
	public:
		CPasswordBox();
		virtual std::string GetText();
		virtual std::string GetPassword();
		virtual void SetPasswordChar(char Letter);
	};
}

#endif