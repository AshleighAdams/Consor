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
	};
};

#endif