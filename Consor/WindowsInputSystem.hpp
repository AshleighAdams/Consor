#ifndef CONSOR_WINDOWSINPUTSYSTEM_H
#define CONSOR_WINDOWSINPUTSYSTEM_H

#include "InputSystem.hpp"

namespace Consor
{
	namespace Input
	{
		class WindowsInputSystem : public IInputSystem
		{
		public:
			WindowsInputSystem();
			virtual bool KeyWaiting();
			virtual Key GetKeyPress();
			virtual bool ControlDown();
			virtual bool ShiftDown();
		};
	}
}

#endif