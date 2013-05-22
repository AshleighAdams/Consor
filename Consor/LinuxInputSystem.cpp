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

Key LinuxInputSystem::GetKeyPress() // these below arn't properly implimented
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
	
	return (Key)ch;
}

bool LinuxInputSystem::ControlDown()
{
	return false;
}

bool LinuxInputSystem::ShiftDown()
{
	return false;
}
