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
#include "Containers/ScrollContainer.hpp"

using namespace std;

int main(int count, char** values)
{
	Consor::CLable lbl1;
	lbl1.SetText("One two three, four five six, seven ate nine! ten elleven twelve, thirtenn four-teen fif-teen six-teen seven-teen eight-teen nine-teen twenty, twenty-one twenty-two twnety three fuck this this is just some text, i can't be arsed to continue the numbers...");
	lbl1.ForceResize(Consor::CSize(20, 1));

	Consor::CFlowContainer flow(Consor::CFlowContainer::FlowAxis::Vertical, 0.0);
	flow.AddControl(lbl1);
	flow.AddControl(lbl1);

	Consor::CScrollContainer scroll(flow, Consor::CSize(-1, 10));

	Consor::CWindowContainer window(scroll, "Flow Test");

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