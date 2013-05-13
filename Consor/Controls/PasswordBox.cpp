#include "PasswordBox.hpp"
#include "Util/StringUtils.hpp"
#include "Util/Time.hpp"
using namespace Consor;

CPasswordBox::CPasswordBox()
{
	m_Size = CSize(20, 1);
	SetText("");
	SetPasswordChar('*');
}

std::string CPasswordBox::GetText()
{
	std::string ret = GetPassword();

	for(char& c : ret)
		c = '*';

	return ret;
}

std::string CPasswordBox::GetPassword()
{
	return m_Text;
}

void CPasswordBox::SetPasswordChar(char Letter)
{
	m_PasswordChar = Letter;
}