#ifndef CONTROLS_HSCROLL_H
#define CONTROLS_HSCROLL_H

#include "../Control.hpp"
#include <string>

namespace Consor
{
	class HorizontalScrollbar : public CControl
	{
	protected:
		double _Value;
		double _ChangeSize;
	public:
		HorizontalScrollbar(const Size& Size);
		HorizontalScrollbar();
		void SetPercent(double Percent);
		double GetPercent();
		void SetChangeSize(double ChangeSize);
		double GetBarSize();
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
	};
}

#endif