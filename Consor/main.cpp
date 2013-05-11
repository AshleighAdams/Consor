#include <iostream>
#include <thread>
#include <mutex>

#include "WindowsConsoleRenderer.hpp"
#include "Util/StringUtils.hpp"
#include "Util/Time.hpp"
#include "Util/FrequencyController.hpp"
#include "WindowsInputSystem.hpp"

#include "Controls/Label.hpp"
#include "Controls/Button.hpp"
#include "Controls/TextBox.hpp"
#include "Controls/PasswordBox.hpp"
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
	std::mutex main_mutex;

	Consor::CLabel msg;
	msg.SetText("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas tempor metus sed ligula tempor tincidunt. Nullam quis condimentum augue. Nulla varius, nunc venenatis molestie egestas, neque lorem bibendum dui, vitae placerat magna nunc at nulla. In ultricies lectus quis purus bibendum eget ullamcorper metus tempus. Phasellus pulvinar, est sit amet auctor tempus, turpis nisl cursus mauris, vitae hendrerit felis tellus eu turpis. Vestibulum id leo sed magna vehicula aliquet. Fusce viverra auctor augue ut rutrum. Quisque quis nisl non turpis sollicitudin rutrum sit amet eget libero. Donec pretium egestas ante, eu aliquam mi porttitor quis.");
	msg.ForceResize(Consor::CSize(36, 1));
	Consor::CScrollContainer msg_scroll(msg, Consor::CSize(-1, 10));
	
	Consor::CFlowContainer button_flow(Consor::CFlowContainer::FlowAxis::Horizontal, 1);
	Consor::CButton ok, cancel;
	ok.SetText("OK");
	cancel.SetText("Cancel");

	button_flow.AddControl(ok);
	button_flow.AddControl(cancel);

	Consor::CAlignContainer button_flow_align(button_flow, Consor::CSize(),
		Consor::CAlignContainer::Axis::Horizotal, Consor::CAlignContainer::Align::Center);

	Consor::CFlowContainer main_flow(Consor::CFlowContainer::FlowAxis::Vertical, 1);
	main_flow.AddControl(msg_scroll);
	main_flow.AddControl(button_flow_align);

	

	Consor::CFlowContainer flow_tests(Consor::CFlowContainer::FlowAxis::Horizontal, 4.0);
	Consor::CFlowContainer flow_tests_lables(Consor::CFlowContainer::FlowAxis::Vertical, 0.0);
	Consor::CFlowContainer flow_tests_controls(Consor::CFlowContainer::FlowAxis::Vertical, 0.0);

	flow_tests.AddControl(flow_tests_lables);
	flow_tests.AddControl(flow_tests_controls);

	// the tests subjects
	
	Consor::CLabel lbl_left, lbl_right;
	lbl_left.SetText("Control Name");
	lbl_right.SetText("Control");

	Consor::CLabel lbl_left_empty, lbl_right_empty;
	lbl_left_empty.SetText("");
	lbl_right_empty.SetText("");

	flow_tests_lables.AddControl(lbl_left);
	flow_tests_lables.AddControl(lbl_left_empty);
	flow_tests_controls.AddControl(lbl_right);
	flow_tests_controls.AddControl(lbl_right_empty);


	// Textbox test
	Consor::CLabel lbl_tb;
	Consor::CTextBox tb;
	lbl_tb.SetText("TextBox:");
	tb.ForceResize(Consor::CSize(20, 1));

	flow_tests_lables.AddControl(lbl_tb);
	flow_tests_controls.AddControl(tb);

	// Button test
	Consor::CLabel lbl_btn;
	Consor::CButton btn;
	lbl_btn.SetText("Button:");
	btn.SetText("Button");

	flow_tests_lables.AddControl(lbl_btn);
	flow_tests_controls.AddControl(btn);

	//HScrollbar test
	Consor::CLabel lbl_scroll;
	Consor::CHorizontalScrollbar scroll;
	lbl_scroll.SetText("HScrollbar:");
	scroll.ForceResize(Consor::CSize(20, 1));
	scroll.SetPercent(0.5);

	flow_tests_lables.AddControl(lbl_scroll);
	flow_tests_controls.AddControl(scroll);

	// PasswordBox test
	Consor::CLabel lbl_pb;
	Consor::CPasswordBox pb;
	lbl_pb.SetText("PasswordBox:");
	pb.ForceResize(Consor::CSize(20, 1));

	flow_tests_lables.AddControl(lbl_pb);
	flow_tests_controls.AddControl(pb);

	// end test
	main_flow.AddControl(flow_tests);

	button_flow_align.ForceResize(main_flow.Size());
	Consor::CWindowContainer window(main_flow, "Consor Test");


	thread input_thread([&]()
	{
		Consor::Util::CFrequencyController input_fc("input", 30);

		Consor::Input::CWindowsInputSystem input;

		while(true)
		{
			if(input_fc.Check() && input.KeyWaiting())
			{
				main_mutex.lock();
				
				Consor::Input::Key kp = input.GetKeyPress();
				window.HandleInput(kp);
				
				main_mutex.unlock();
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
				main_mutex.lock();

				renderer.Clear(Consor::CColour());
		
				Consor::CSize size = window.Size();
				Consor::CSize rsize = renderer.RenderSize();

				Consor::CVector pos = Consor::CVector(rsize.Width / 2.0, rsize.Height / 2.0) - Consor::CVector(size.Width / 2, size.Height / 2);

				renderer.PushRenderBounds(pos, window.Size());
				window.Draw(renderer, true, skin);
				renderer.PopRenderBounds();
		
				renderer.FlushToScreen();

				main_mutex.unlock();
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