#ifndef CONSOR_LINUXINPUTSYSTEM_H
#define CONSOR_LINUXINPUTSYSTEM_H

#include "InputSystem.hpp"
#include <termios.h>

namespace Consor
{
	namespace Input
	{
		class LinuxInputSystem : public IInputSystem
		{
			bool _ShiftDown;
			bool _ControlDown;
			termios _Old;
		public:
			LinuxInputSystem();
			virtual ~LinuxInputSystem();
			bool KeyWaiting();
			Key GetKeyPress();
			bool ControlDown();
			bool ShiftDown();
		};
	};
}
#endif
