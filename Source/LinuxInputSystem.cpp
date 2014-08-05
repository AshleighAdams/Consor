#include "LinuxInputSystem.hpp"

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

using namespace Consor;
using namespace Consor::Input;

LinuxInputSystem::LinuxInputSystem()
{
	termios newt;
	int ch;
	
	tcgetattr(STDIN_FILENO, &_Old);
	newt = _Old;
	newt.c_lflag &= ~(ICANON | ECHO);
	
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

LinuxInputSystem::~LinuxInputSystem()
{
	tcsetattr(STDIN_FILENO, TCSANOW, &_Old);
}

bool LinuxInputSystem::KeyWaiting()
{
	termios oldt, newt;
	int ch, oldf;
		
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	
		ch = getchar();
	
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	
	if(ch != EOF)
	{
		ungetc(ch, stdin);
		return true;
	}
	
	return false;
}

Key LinuxInputSystem::GetKeyPress() // these below arn't properly implimented
{
	_ShiftDown = _ControlDown = false;
	
	int ch = getchar();
	
	switch(ch)
	{
	case 127:
		return Key::Backspace;
	case 10:
		return Key::Enter;
	case '[':
		ch = getchar();
		
		switch(ch)
		{
		case 'A':
			return Key::Up;
		case 'B':
			return Key::Down;
		case 'C':
			return Key::Right;
		case 'D':
			return Key::Left;
			
		case '1':
		{
			getchar(); // read the ;
			ch = getchar();
			
			switch(ch)
			{
			case '2': // shift was pushed
				_ShiftDown = true;
				break;
			case '5': // ctrl was pushed
				_ControlDown = true;
				break;
			default: // unknown
				break;
			}
			
			ch = getchar();
			switch(ch)
			{
			case 'A':
				return Key::Up;
			case 'B':
				return Key::Down;
			case 'C':
				return Key::Right;
			case 'D':
				return Key::Left;
			default:
				return (Key)ch;
			}
		}
		default:
			return (Key)ch;
		}
	default:
		return (Key)ch;
	}
	return (Key)ch;
}

bool LinuxInputSystem::ControlDown()
{
	return _ControlDown;
}

bool LinuxInputSystem::ShiftDown()
{
	return _ShiftDown;
}
