#ifndef CONTROLS_LABLE_H
#define CONTROLS_LABLE_H

#include "Control.hpp"
#include <string>

namespace Consor
{
	class CLable : public CControl
	{
		std::string m_Text;
		std::string m_SetText;
	public:
		CLable();
		void SetText(std::string Text);
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
		virtual void ForceResize(const CSize& Size);
	};
}

#endif