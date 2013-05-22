#ifndef CONTROLS_VPROGRESSBAR_H
#define CONTROLS_VPROGRESSBAR_H

#include "../Control.hpp"
#include "../Util/Hooks.hpp"

#include "ProgressBar.hpp"
#include <string>

namespace Consor
{
	class VerticalProgressBar : public ProgressBar
	{
		bool _GraphMode, _ColourOverrid;
		size_t _GraphIndex;
		Colour _CustomColour;
	public:
		Hook<double, size_t> Click;

		VerticalProgressBar();
		void SetGraphMode(size_t Index); // allows focusing, and input
		void ColourOveride(const Consor::Colour& col);
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
		virtual bool HandleInput(Input::Key Key, Input::IInputSystem& System);
		virtual bool CanFocus();
	};
}

#endif