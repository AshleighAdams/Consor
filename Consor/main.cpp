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
	lbl.SetText("Hello, world; how are you on this fine day?");
	lbl.ForceResize(Consor::CSize(13, 5));

	Consor::CDefaultSkin skin;

	double scroll = 0;

	while(true)
	{
		renderer.Clear(Consor::CColour());
		
		{
			renderer.PushRenderBounds(Consor::CVector(5, 5), lbl.Size());
			renderer.PushRenderBounds(Consor::CVector(0, scroll), renderer.RenderSize());
			{
				lbl.Draw(renderer, false, skin);
			}
			renderer.PopRenderBounds();
			renderer.PopRenderBounds();
		}

		renderer.FlushToScreen();
	}


	return 0;
}