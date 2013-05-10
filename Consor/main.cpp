#include <iostream>

#include "WindowsConsoleRenderer.hpp"
#include "Util/StringUtils.hpp"
#include "Util/Time.hpp"

#include "Controls/Lable.hpp"
#include "Containers/WindowContainer.hpp"
#include "Containers/BorderContainer.hpp"
#include "Containers/FlowContainer.hpp"

using namespace std;

int main(int count, char** values)
{
	Consor::Console::CWindowsConsoleRenderer renderer; 
	Consor::CDefaultSkin skin;



	Consor::CLable lbl1;
	lbl1.SetText("Hello, world; how are you on this fine day?");
	lbl1.ForceResize(Consor::CSize(20, 5));

	Consor::CLable lbl2;
	lbl2.SetText("Second lable is here.");

	Consor::CFlowContainer flow(Consor::CFlowContainer::FlowAxis::Horizontal, 1.0);
	flow.AddControl(lbl1);
	flow.AddControl(lbl2);

	Consor::CWindowContainer window(flow, "Flow Test");


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