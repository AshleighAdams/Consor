#include <iostream>

#include "WindowsConsoleRenderer.hpp"
#include "Util/StringUtils.hpp"
#include "Util/Time.hpp"

#include "Controls/Lable.hpp"
#include "Containers/WindowContainer.hpp"
#include "Containers/BorderContainer.hpp"

using namespace std;

int main(int count, char** values)
{
	Consor::Console::CWindowsConsoleRenderer renderer; 
	Consor::CDefaultSkin skin;

	Consor::CLable lbl;
	lbl.SetText("Hello, world; how are you on this fine day?");
	lbl.ForceResize(Consor::CSize(20, 5));

	Consor::CBorderContainer border(lbl, 2);

	Consor::CWindowsContainer window(border, "Hello");

	while(true)
	{
		renderer.Clear(Consor::CColour());
		
		Consor::CSize size = window.Size();
		Consor::CSize rsize = renderer.RenderSize();

		Consor::CVector pos = Consor::CVector(rsize.Width / 2.0, rsize.Height / 2.0) - Consor::CVector(size.Width / 2, size.Height / 2);

		renderer.PushRenderBounds(pos, window.Size());
		window.Draw(renderer, true, skin);
		
		renderer.FlushToScreen();
	}


	return 0;
}