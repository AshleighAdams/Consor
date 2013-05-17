#ifndef CONTROL_H
#define CONTROL_H

#include "Units.hpp"
#include "Skin.hpp"
#include "ConsoleRenderer.hpp"
#include "InputSystem.hpp"

namespace Consor
{
	class Control
	{
	protected:
		Consor::Size _Size;
	public:
		virtual ~Control(){}
		virtual Size GetSize();
		virtual void OnResize(const Size& Size);
		virtual void ForceResize(const Size& Size);
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
		virtual bool HandleInput(Input::Key Key, Input::IInputSystem& System);
		virtual bool CanFocus();
	};
};

#endif