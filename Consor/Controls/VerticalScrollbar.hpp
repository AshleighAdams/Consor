#ifndef CONTROLS_VSCROLL_H
#define CONTROLS_VSCROLL_H

#include "../Control.hpp"
#include <string>

namespace Consor
{
	class VerticalScrollbar : public Control
	{
	protected:
		double _Value;
		double _ChangeSize;
		double _ScrollRegionSize;
	public:
		VerticalScrollbar(const Size& Size);
		VerticalScrollbar();
		void SetPercent(double Percent);
		double GetPercent();
		void SetChangeSize(double ChangeSize);
		void SetScrollRegionSize(double RegionSize);
		double GetBarSize();
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
	};
}

#endif