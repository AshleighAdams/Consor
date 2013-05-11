#ifndef CONTROLS_HSCROLL_H
#define CONTROLS_HSCROLL_H

#include "Control.hpp"
#include <string>

namespace Consor
{
	class CHorizontalScrollbar : public CControl
	{
	protected:
		double m_Value;
	public:
		CHorizontalScrollbar(const CSize& Size);
		void SetPercent(double Percent);
		double GetPercent();
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
	};
}

#endif