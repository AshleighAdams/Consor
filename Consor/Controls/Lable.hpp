#ifndef CONTROLS_LABLE_H
#define CONTROLS_LABLE_H

#include "Control.hpp"
#include <string>

namespace Consor
{
	class CLable : CControl
	{
		std::string m_Text;
	public:
		CLable();
		void SetText(std::string Text);
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
	};
}

#endif