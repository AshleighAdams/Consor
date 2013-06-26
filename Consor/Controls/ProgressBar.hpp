#ifndef CONTROLS_PROGRESSBAR_H
#define CONTROLS_PROGRESSBAR_H

#include "../Control.hpp"
#include <string>

namespace Consor
{
	class ProgressBar : public Control
	{
	protected:
		double _Percent;
	public:
		ProgressBar();
		void SetPercent(double Progress);
		double GetPercent();
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
	};
}

#endif