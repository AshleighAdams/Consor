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
#include "Controls/CheckBox.hpp"
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
	/*
	Consor::Input::CWindowsInputSystem input;
	Consor::Console::CWindowsConsoleRenderer renderer; 
	Consor::CDefaultSkin skin;
	skin.SetRendererColours(renderer);

	renderer.Clear(Consor::CColour());
	renderer.DrawBox(Consor::CVector(), Consor::CSize(4, 4), Consor::CColour(1, 0.5, 0));

	renderer.FlushToScreen();

	return 0;
	*/
	std::mutex main_mutex;
	bool main_exit = false;
	Consor::Console::CWindowsConsoleRenderer renderer;

	Consor::CWindowContainer* pWindow = nullptr;

	thread program_thread([&]()
	{
		Consor::Input::CWindowsInputSystem input;
		Consor::CHackerSkin skin(renderer);

		auto draw = [&]()
		{
			if(!pWindow)
				return;

			main_mutex.lock();
			double start = Consor::Util::GetTime();

			renderer.Clear(Consor::CColour());
		
			Consor::CSize size = pWindow->Size();
			Consor::CSize rsize = renderer.RenderSize();

			Consor::CVector pos = Consor::CVector(rsize.Width / 2.0, rsize.Height / 2.0) - Consor::CVector(size.Width / 2, size.Height / 2);

			renderer.PushRenderBounds(pos, pWindow->Size());
			pWindow->Draw(renderer, true, skin);
			renderer.PopRenderBounds();
		
			renderer.FlushToScreen();

			double span = Consor::Util::GetTime() - start;
			renderer.SetTitle(Consor::Util::FormatString("Rendered in %ms", (int)(span * 1000.0)));
			main_mutex.unlock();
		};

		thread input_thread([&]() // so stuff still blinks, and updates slowly
		{
			while(!main_exit)
			{
				draw();
				Consor::Util::Sleep(0.5);
			}
		});


		while(!main_exit)
		{
			// input
			Consor::Input::Key kp = input.GetKeyPress();

			if(pWindow)
				pWindow->HandleInput(kp);

			// draw
			draw();
		}

		input_thread.join();
	});

	// the login box (not for real, just a test)
	{
		using namespace Consor;

		CFlowContainer flow_table(CFlowContainer::FlowAxis::Horizontal, 4);
		CFlowContainer flow_table_col1(CFlowContainer::FlowAxis::Vertical, 0);
		CFlowContainer flow_table_col2(CFlowContainer::FlowAxis::Vertical, 0);

		flow_table.AddControl(flow_table_col1);
		flow_table.AddControl(flow_table_col2);

		CLabel lbl_un, lbl_pw;
		lbl_un.SetText("Username:");
		lbl_pw.SetText("Password:");
		
		flow_table_col1.AddControl(lbl_un);
		flow_table_col1.AddControl(lbl_pw);

		CTextBox un; CPasswordBox pw;
		un.ForceResize(CSize(20, 1));
		un.SetText("test");
		pw.ForceResize(CSize(20, 1));

		flow_table_col2.AddControl(un);
		flow_table_col2.AddControl(pw);

		CFlowContainer flow_buttons(CFlowContainer::FlowAxis::Horizontal, 1.0);
		CButton ok, cancel;
		ok.SetText("Login");
		cancel.SetText("Exit");
		flow_buttons.AddControl(ok);
		flow_buttons.AddControl(cancel);

		CAlignContainer align_flow_buttons(flow_buttons, CSize(), CAlignContainer::Axis::Horizotal, CAlignContainer::Align::Center);

		CLabel msg;
		CAlignContainer align_msg(msg, CSize(), CAlignContainer::Axis::Horizotal, CAlignContainer::Align::Center);

		CFlowContainer flow_main(CFlowContainer::FlowAxis::Vertical, 0);
		flow_main.AddControl(flow_table);
		flow_main.AddControl(align_msg);
		flow_main.AddControl(align_flow_buttons);

		align_flow_buttons.ForceResize(flow_main.Size());
		align_msg.ForceResize(flow_main.Size());
		CWindowContainer window(flow_main, "Login");
		
		bool loggedin = false;

		ok.Click += [&]()
		{
			if(un.GetText() == "test" && pw.GetPassword() == "test")
				loggedin = true;
			else 
			{
				msg.SetText("Invalid user/password");
				pw.SetText("");
			}
		};
		cancel.Click += [&]()
		{
			main_exit = true;
		};

		pWindow = &window;

		while(!loggedin)
		{
			Util::Sleep(0.1);
			if(main_exit)
			{
				program_thread.join();
				return 0;
			}
		}

		pWindow = nullptr;
	}

	{
		Consor::CLabel msg;
		//msg.SetText("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas tempor metus sed ligula tempor tincidunt. Nullam quis condimentum augue. Nulla varius, nunc venenatis molestie egestas, neque lorem bibendum dui, vitae placerat magna nunc at nulla. In ultricies lectus quis purus bibendum eget ullamcorper metus tempus. Phasellus pulvinar, est sit amet auctor tempus, turpis nisl cursus mauris, vitae hendrerit felis tellus eu turpis. Vestibulum id leo sed magna vehicula aliquet. Fusce viverra auctor augue ut rutrum. Quisque quis nisl non turpis sollicitudin rutrum sit amet eget libero. Donec pretium egestas ante, eu aliquam mi porttitor quis.");
	
		msg.SetText(Consor::Util::FormatString("The current renderer is %, version string \"%\".\n\nSupports Unicode: %\nMaximium supported colours: %", renderer.RendererName(), renderer.VersionString(), renderer.GetCharInformation(Consor::CVector())->SupportsUnicode() ? "true" : "false", renderer.MaxColours()));

		msg.ForceResize(Consor::CSize(36, 1));
		Consor::CScrollContainer msg_scroll(msg, Consor::CSize(-1, 10));
	
		Consor::CFlowContainer button_flow(Consor::CFlowContainer::FlowAxis::Horizontal, 1);
		Consor::CButton exit;
		exit.SetText("Exit");

		button_flow.AddControl(exit);

		exit.Click += [&]()
		{
			main_exit = true;
		};

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
		size_t clicked = 0;
		btn.Click += [&]()
		{
			clicked++;
			btn.SetText(Consor::Util::FormatString("Button (pushed x%)", clicked));
		};

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

		// PasswordBox test
		Consor::CLabel lbl_cb;
		Consor::CCheckBox cb;
		lbl_cb.SetText("CheckBox:");
		cb.SetText("CheckBox Text");

		flow_tests_lables.AddControl(lbl_cb);
		flow_tests_controls.AddControl(cb);

		// end test
		main_flow.AddControl(flow_tests);

		button_flow_align.ForceResize(main_flow.Size());
		Consor::CWindowContainer window(main_flow, "Consor Test");
		pWindow = &window;
	
		while(!main_exit)
		{
			Consor::Util::Sleep(1.0);
		}

		pWindow = nullptr;
		program_thread.join();
		return 0;
	}

	main_exit = true;
	program_thread.join();
	return 0;
}