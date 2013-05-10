#include <iostream>

#include "WindowsConsoleRenderer.hpp"
#include "Util/StringUtils.hpp"
#include "Util/Time.hpp"

#include "Controls/Lable.hpp"
#include "Containers/WindowContainer.hpp"
#include "Containers/BorderContainer.hpp"
#include "Containers/FlowContainer.hpp"
#include "Containers/AlignContainer.hpp"

using namespace std;

int main(int count, char** values)
{
	Consor::Console::CWindowsConsoleRenderer renderer; 
	Consor::CDefaultSkin skin;



	Consor::CLable lbl1;
	lbl1.SetText("Some message can go here, it can be anything, IDGAS really");

	Consor::CLable pretend_button_ok;
	pretend_button_ok.SetText("<OK>");

	Consor::CLable pretend_button_cancel;
	pretend_button_cancel.SetText("<Cancel>");

	Consor::CFlowContainer flow(Consor::CFlowContainer::FlowAxis::Vertical, 1.0);
	Consor::CFlowContainer flow_buttons(Consor::CFlowContainer::FlowAxis::Horizontal, 2.0);

	flow_buttons.AddControl(pretend_button_ok);
	flow_buttons.AddControl(pretend_button_cancel);

	

	Consor::CAlignContainer allign_buttons(flow_buttons, Consor::CSize(), 
		Consor::CAlignContainer::Axis::Horizotal, Consor::CAlignContainer::Align::Center);

	flow.AddControl(lbl1);
	flow.AddControl(allign_buttons);

	allign_buttons.ForceResize(flow.Size());

	Consor::CWindowContainer window(flow, "Flow Test");


	while(true)
	{
		renderer.Clear(Consor::CColour());
		
		Consor::CSize size = window.Size();
		Consor::CSize rsize = renderer.RenderSize();

		Consor::CVector pos = Consor::CVector(rsize.Width / 2.0, rsize.Height / 2.0) - Consor::CVector(size.Width / 2, size.Height / 2);

		renderer.PushRenderBounds(pos, window.Size());
		window.Draw(renderer, true, skin);
		renderer.PopRenderBounds();
		
		renderer.FlushToScreen();
	}


	return 0;
}