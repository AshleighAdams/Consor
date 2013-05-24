#ifndef CONTROLS_HSCROLL_H
#define CONTROLS_HSCROLL_H

#include "../Control.hpp"
#include <string>

namespace Consor
{
	class HorizontalScrollbar : public Control
	{
	protected:
		double _Value;
		double _ChangeSize;
		double _ScrollRegionSize;
	public:
		HorizontalScrollbar(const Size& Size);
		HorizontalScrollbar();
		void SetPercent(double Percent);
		double GetPercent();
		void SetChangeSize(double ChangeSize);
		void SetScrollRegionSize(double RegionSize);
		double GetBarSize();
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
	};
}

#endif