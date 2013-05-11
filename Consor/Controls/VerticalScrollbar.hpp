#ifndef CONTROLS_VSCROLL_H
#define CONTROLS_VSCROLL_H

#include "Control.hpp"
#include <string>

namespace Consor
{
	class CVerticalScrollbar : public CControl
	{
	protected:
		double m_Value;
	public:
		CVerticalScrollbar(const CSize& Size);
		CVerticalScrollbar();
		void SetPercent(double Percent);
		double GetPercent();
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
	};
}

#endif