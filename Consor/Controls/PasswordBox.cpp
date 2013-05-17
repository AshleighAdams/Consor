#include "PasswordBox.hpp"
#include "Util/StringUtils.hpp"
#include "Util/Time.hpp"
using namespace Consor;

PasswordBox::PasswordBox()
{
	_Size = Size(20, 1);
	this->SetText("");
	SetPasswordChar('*');
}

std::string PasswordBox::GetText()
{
	std::string ret = this->GetPassword();

	for(char& c : ret)
		c = '*';

	return ret;
}

std::string PasswordBox::GetPassword()
{
	return _Text;
}

void PasswordBox::SetPasswordChar(char Letter)
{
	_PasswordChar = Letter;
}