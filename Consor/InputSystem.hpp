#ifndef CONSOR_INPUTSYSTEM_H
#define CONSOR_INPUTSYSTEM_H

namespace Consor
{
	namespace Input
	{
		enum Key
		{
			Escape  = 0,
			Enter = 1,
			Space = 32,

			Insert = 2,
			Home = 3,
			PageUp = 4,
			Delete = 5,
			End = 6,
			PageDown = 7,

			Up = 14,
			Down = 15,
			Left = 16,
			Right = 17,

			F1 = 18,
			F2 = 19,
			F3 = 20,
			F4 = 21,
			F5 = 22,
			F6 = 23,
			F7 = 24,
			F8 = 25,
			F9 = 26,
			F10 = 27,
			F11 = 28,
			F12 = 29,

			NumpadDelete = 30,
			Numpad0 = 31,
			Numpad1 = 127,
			Numpad2 = 128,
			Numpad3 = 129,
			Numpad4 = 130,
			Numpad5 = 131,
			Numpad6 = 132,
			Numpad7 = 133,
			Numpad8 = 134,
			Numpad9 = 135
		};

		//extern bool KeyWaiting();
		//extern Key GetKeyPress();

		class IInputSystem // Jookia is the one who wanted this to be instanced
		{
		public:
			virtual bool KeyWaiting() = 0;
			virtual Key GetKeyPress() = 0;
		};
	}
}

#endif