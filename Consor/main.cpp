#include <iostream>
#include <thread>

#include "WindowsConsoleRenderer.hpp"
#include "Util/StringUtils.hpp"
#include "Util/Time.hpp"
#include "Util/FrequencyController.hpp"
#include "WindowsInputSystem.hpp"

#include "Controls/Lable.hpp"
#include "Controls/Button.hpp"
#include "Controls/HorizontalScrollbar.hpp"
#include "Controls/VerticalScrollbar.hpp"

#include "Containers/WindowContainer.hpp"
#include "Containers/BorderContainer.hpp"
#include "Containers/FlowContainer.hpp"
#include "Containers/AlignContainer.hpp"

using namespace std;

int main(int count, char** values)
{
	Consor::CLable lbl1;
	lbl1.SetText("Some message can go here, it can be anything, IDGAS really");

	Consor::CButton button_ok;
	button_ok.SetText("OK");

	button_ok.Click += [&]()
	{
		button_ok.SetText("No, fuck you.");
	};

	Consor::CButton button_cancel;
	button_cancel.SetText("Cancel");

	button_cancel.Click += [&]()
	{
		button_cancel.SetText("WHAT DID I FUCKING TELL YOU?");
	};

	Consor::CVerticalScrollbar scrollbar(Consor::CSize(1, 10));
	scrollbar.SetPercent(0.5);

	Consor::CFlowContainer flow(Consor::CFlowContainer::FlowAxis::Vertical, 1.0);
	Consor::CFlowContainer flow_buttons(Consor::CFlowContainer::FlowAxis::Horizontal, 2.0);

	flow_buttons.AddControl(button_ok);
	flow_buttons.AddControl(button_cancel);

	Consor::CAlignContainer allign_buttons(flow_buttons, Consor::CSize(), 
		Consor::CAlignContainer::Axis::Horizotal, Consor::CAlignContainer::Align::Center);

	flow.AddControl(lbl1);
	flow.AddControl(scrollbar);
	flow.AddControl(allign_buttons);

	allign_buttons.ForceResize(flow.Size());

	Consor::CWindowContainer window(flow, "Flow Test");

	thread input_thread([&]()
	{
		Consor::Util::CFrequencyController input_fc("input", 30);

		Consor::Input::CWindowsInputSystem input;

		while(true)
		{
			if(input_fc.Check() && input.KeyWaiting())
			{
				Consor::Input::Key kp = input.GetKeyPress();
				window.HandleInput(kp);
			}
			else
			{
				Consor::Util::Sleep(1.0 / 30.0);
			}
		}
	});

	thread draw_thread([&]()
	{
		Consor::Util::CFrequencyController draw_fc("draw", 24);
		Consor::Console::CWindowsConsoleRenderer renderer; 
		Consor::CDefaultSkin skin;

		while(true)
		{
			if(draw_fc.Check())
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
			else
			{
				Consor::Util::Sleep(1.0 / 24.0);
			}

			
		}
	});

	while(true)
	{
		Consor::Util::Sleep(0.25);
	}


	return 0;
}