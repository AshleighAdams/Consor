#ifndef CONSOR_PROMPTS_H
#define CONSOR_PROMPTS_H

#ifdef MessageBox // Windows MessageBox(A/W)
#undef MessageBox
#endif

#include <string>
#include <list>

namespace Consor
{
	namespace Util
	{
		/// Display a message box to the user.
		/// \return The button the user clicked on.
		extern std::string MessageBox(const std::string& Message, const std::string& Title, std::list<std::string> Buttons);
		/// The user can select a value from a list of choices.
		/// \return The selection the user entered.
		extern std::string ChoiceList(const std::string& Message, const std::string& Title, std::list<std::string> Choices);
		/// The user can enter a string.
		/// \return The string the user entered.
		extern std::string InputBox(const std::string& Message, const std::string& Title);
	};
};

#endif
