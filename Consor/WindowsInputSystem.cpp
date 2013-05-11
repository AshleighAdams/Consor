#include "WindowsInputSystem.hpp"

#include <conio.h>

using namespace Consor::Input;

CWindowsInputSystem::CWindowsInputSystem()
{
}

bool CWindowsInputSystem::KeyWaiting()
{
	return _kbhit();
}

Key CWindowsInputSystem::GetKeyPress()
{
#ifndef WIN32
	int cnt = kbhit(); // for ANSI escapes processing
#endif
	int k = _getch();
	switch(k) 
	{
		case 0: 
		{
			int kk;
			switch (kk = _getch()) 
			{
				case 71: return Key::Numpad7;
				case 72: return Key::Numpad8;
				case 73: return Key::Numpad9;
				case 75: return Key::Numpad4;
				case 77: return Key::Numpad6;
				case 79: return Key::Numpad1;
				case 80: return Key::Numpad4;
				case 81: return Key::Numpad3;
				case 82: return Key::Numpad0;
				case 83: return Key::NumpadDelete;
				default: return (Key)(kk-59+ (int)Key::F1); // Function keys
			}
		}
		case 224: 
		{
			int kk;
			switch (kk = _getch()) 
			{
				case 71: return Key::Home;
				case 72: return Key::Up;
				case 73: return Key::PageUp;
				case 75: return Key::Left;
				case 77: return Key::Right;
				case 79: return Key::End;
				case 80: return Key::Down;
				case 81: return Key::PageDown;
				case 82: return Key::Insert;
				case 83: return Key::Delete;
				default: return (Key)(kk-123+(int)Key::F1); // Function keys
			}
		}
		case 13: return Key::Enter;
#ifdef WIN32
		case 27: return Key::Escape;
#else // WIN32
		case 155: // single-character CSI
		case 27: {
			// Process ANSI _getch sequences
			if (cnt >= 3 && getch() == '[') 
			{
				switch (k = _getch()) 
				{
					case 'A': return Key::Up;
					case 'B': return Key::Down;
					case 'C': return Key::Right;
					case 'D': return Key::Left;
				}
			}
			else
				return Key::Escape;
		}
#endif // WIN32
		default: return (Key)k;
	}
}
