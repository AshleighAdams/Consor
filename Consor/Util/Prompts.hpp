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
		extern std::string MessageBox(const std::string& Message, const std::string& Title, std::list<std::string> Buttons);
		extern std::string ChoiceList(const std::string& Message, const std::string& Title, std::list<std::string> Choices);
		extern std::string InputBox(const std::string& Message, const std::string& Title);
	};
};

#endif