#ifndef CONTROLS_VPROGRESSBAR_H
#define CONTROLS_VPROGRESSBAR_H

#include "../Control.hpp"
#include "../Util/Hooks.hpp"

#include "ProgressBar.hpp"
#include <string>

namespace Consor
{
	class VerticalProgressBar : public CProgressBar
	{
		bool _GraphMode;
		size_t _GraphIndex;
	public:
		CHook<double, size_t> Click;

		VerticalProgressBar();
		void SetGraphMode(size_t Index); // allows focusing, and input
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
		virtual bool HandleInput(Input::Key Key, Input::IInputSystem& System);
		virtual bool CanFocus();
	};
}

#endif