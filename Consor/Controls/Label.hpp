#ifndef CONTROLS_LABLE_H
#define CONTROLS_LABLE_H

#include "../Control.hpp"
#include <string>

namespace Consor
{
	class CLabel : public CControl
	{
	protected:
		std::string m_Text;
		std::string m_SetText;
	public:
		CLabel();
		void SetText(const std::string& Text);
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
		virtual void ForceResize(const CSize& Size);
	};
}

#endif