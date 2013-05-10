#include <iostream>

#include "WindowsConsoleRenderer.hpp"
#include "Util/StringUtils.hpp"
#include "Util/Time.hpp"

#include "Controls/Lable.hpp"

using namespace std;

int main(int count, char** values)
{
	Consor::Console::CWindowsConsoleRenderer renderer; 

	Consor::CLable lbl;
	lbl.SetText("Hello, world");

	Consor::CDefaultSkin skin;

	while(true)
	{
		renderer.Clear(Consor::CColour());
		
		{
			renderer.PushRenderBounds(Consor::CVector(5, 5), lbl.Size());
			lbl.Draw(renderer, false, skin);
			renderer.PopRenderBounds();
		}

		renderer.FlushToScreen();
	}


	return 0;
}