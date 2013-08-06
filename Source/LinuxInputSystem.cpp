#include "LinuxInputSystem.hpp"

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

using namespace Consor;
using namespace Consor::Input;

bool LinuxInputSystem::KeyWaiting()
{
	termios oldt, newt;
	int ch, oldf;
	
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	
	ch = getchar();
	
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	
	if(ch != EOF)
	{
		ungetc(ch, stdin);
		return true;
	}
	
	return false;
}

int get_pressed()
{
	termios oldt, newt;
	int ch;
	
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	{
		ch = getchar();
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	
	return ch;
}

Key LinuxInputSystem::GetKeyPress() // these below arn't properly implimented
{
	_ShiftDown = _ControlDown = false;
	
	int ch = get_pressed();
	
	switch(ch)
	{
	case 127:
		return Key::Backspace;
	case 10:
		return Key::Enter;
	case '[':
		ch = get_pressed();
		
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
			get_pressed(); // read the ;
			ch = get_pressed();
			
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
			
			ch = get_pressed();
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
