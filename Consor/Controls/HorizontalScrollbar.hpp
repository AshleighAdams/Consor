#ifndef CONTROLS_HSCROLL_H
#define CONTROLS_HSCROLL_H

#include "../Control.hpp"
#include <string>

namespace Consor
{
	class CHorizontalScrollbar : public CControl
	{
	protected:
		double m_Value;
		double m_ChangeSize;
	public:
		CHorizontalScrollbar(const CSize& Size);
		CHorizontalScrollbar();
		void SetPercent(double Percent);
		double GetPercent();
		void SetChangeSize(double ChangeSize);
		double GetBarSize();
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
	};
}

#endif