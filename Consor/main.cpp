#include <iostream>
#include <thread>
#include <mutex>

#include "WindowsConsoleRenderer.hpp"
#include "Util/StringUtils.hpp"
#include "Util/Time.hpp"
#include "Util/FrequencyController.hpp"
#include "Util/Math.hpp"
#include "WindowsInputSystem.hpp"

#include "Controls/Label.hpp"
#include "Controls/Button.hpp"
#include "Controls/TextBox.hpp"
#include "Controls/PasswordBox.hpp"
#include "Controls/CheckBox.hpp"
#include "Controls/ProgressBar.hpp"
#include "Controls/HorizontalScrollbar.hpp"
#include "Controls/VerticalScrollbar.hpp"

#include "Containers/WindowContainer.hpp"
#include "Containers/BorderContainer.hpp"
#include "Containers/FlowContainer.hpp"
#include "Containers/AlignContainer.hpp"
#include "Containers/ScrollContainer.hpp"

#include "WindowSystem.hpp"

using namespace std;

class CSaneSkin : public Consor::CDefaultSkin
{
public:
	CSaneSkin(Consor::Console::IConsoleRenderer& Renderer)
	{
		WindowLeft = ' ';
		WindowRight = ' ';

		m_ColourPos = 0;
		Foreground = RequestColour(Renderer, Consor::CColour(0, 0, 0));
		ForegroundShine = RequestColour(Renderer, Consor::CColour(1, 1, 1));
		Background = RequestColour(Renderer, Consor::CColour(0.7, 0.7, 0.7));
		AlternateBackground = RequestColour(Renderer, Consor::CColour(0.3, 0.3, 0.3));
		FocusColour = RequestColour(Renderer, Consor::CColour(0, 0, 1));
		ProgressPercent = RequestColour(Renderer, Consor::CColour(1, 1, 1));
		ProgressForeground = RequestColour(Renderer, Consor::CColour(0, 1, 1));
	};
};

#include <codecvt>
#include <locale>

int main(int count, wchar_t** values)
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
	Consor::WindowSystem::Setup(new Consor::Console::CWindowsConsoleRenderer(),
		new Consor::Input::CWindowsInputSystem());
	Consor::WindowSystem::SetSkin<Consor::CHackerSkin>();

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
		un.SetText("test");

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

		bool close = false;
		cancel.Click += [&]()
		{
			close = true;
		};
		
		WindowSystem::RegisterWindow(window, CVector(-1, -1));

		while(!loggedin)
		{
			Util::Sleep(0.1);
			if(close)
			{
				WindowSystem::Close();
				return 0;
			}
		}
		
		WindowSystem::UnregisterWindow(window);
	}

	// the pretend "logging in" window
	{
		using namespace Consor;

		CLabel consoletext;
		CFlowContainer flow(CFlowContainer::FlowAxis::Vertical, 0);
		flow.AddControl(consoletext);

		CScrollContainer scroll(flow, CSize(40, 8));

		string total = "";
		bool notscrolled = true;
		auto addtext = [&](const std::string& msg)
		{
			WindowSystem::Lock(); // we're messing with something that the main draw thread will be too
			total += msg + "\n";
			consoletext.SetText(total);
			consoletext.ForceResize(scroll.Size() - CSize(1, 1));
			
			if(notscrolled && scroll.ScrollDown())
				notscrolled = false;
			WindowSystem::Unlock();
		};

		CWindowContainer window(scroll, "Logging in");

		WindowSystem::RegisterWindow(window, CVector(-1, -1));

		addtext("commencing the ScrollContainer auto scrolling test..."); Util::Sleep(0.5);
		addtext("connecting to login server..."); Util::Sleep(0.5);
		addtext("logging in..."); Util::Sleep(2);

		string reasons[] = {"information", "datapack", "members", "updates", "earth", "universe", "lorem ipsum", "oxford dictionary"};

		for(int i = 0; i < (sizeof reasons / sizeof reasons[0]); i++)
		{
			addtext(Util::FormatString("downloading \"%\"...", reasons[i])); Util::Sleep(0.75);
		}

		addtext("launching..."); Util::Sleep(1);
		addtext("loading UI..."); Util::Sleep(1);

		CProgressBar progbar;
		progbar.ForceResize(CSize(flow.Size().Width - 2, 1));

		WindowSystem::Lock();
		flow.AddControl(progbar);
		WindowSystem::Unlock();

		for(int i = 0; i < 10; i++)
		{
			progbar.SetPercent((double)i / 10.0);
			WindowSystem::Draw();
			Util::Sleep(0.1);
		}
		Util::Sleep(1);

		WindowSystem::UnregisterWindow(window);
	}

	{
		Consor::CLabel msg;
		msg.SetText("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas tempor metus sed ligula tempor tincidunt. Nullam quis condimentum augue. Nulla varius, nunc venenatis molestie egestas, neque lorem bibendum dui, vitae placerat magna nunc at nulla. In ultricies lectus quis purus bibendum eget ullamcorper metus tempus. Phasellus pulvinar, est sit amet auctor tempus, turpis nisl cursus mauris, vitae hendrerit felis tellus eu turpis. Vestibulum id leo sed magna vehicula aliquet. Fusce viverra auctor augue ut rutrum. Quisque quis nisl non turpis sollicitudin rutrum sit amet eget libero. Donec pretium egestas ante, eu aliquam mi porttitor quis.");
	
		//msg.SetText(Consor::Util::FormatString("The current renderer is %, version string \"%\".\n\nSupports Unicode: %\nMaximium supported colours: %", renderer.RendererName(), renderer.VersionString(), renderer.GetCharInformation(Consor::CVector())->SupportsUnicode() ? "true" : "false", renderer.MaxColours()));

		msg.ForceResize(Consor::CSize(36, 1));
		Consor::CScrollContainer msg_scroll(msg, Consor::CSize(-1, 10));
	
		Consor::CFlowContainer button_flow(Consor::CFlowContainer::FlowAxis::Horizontal, 1);
		Consor::CButton exit;
		exit.SetText("Exit");

		button_flow.AddControl(exit);

		bool close = false;
		exit.Click += [&]()
		{
			close = true;
		};

		Consor::CAlignContainer button_flow_align(button_flow, Consor::CSize(),
			Consor::CAlignContainer::Axis::Horizotal, Consor::CAlignContainer::Align::Center);

		Consor::CFlowContainer main_flow(Consor::CFlowContainer::FlowAxis::Vertical, 1);
		main_flow.AddControl(msg);
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
		flow_tests_controls.AddControl(btn, 20);

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

		// CheckBox test
		Consor::CLabel lbl_cb;
		Consor::CCheckBox cb;
		lbl_cb.SetText("CheckBox:");
		cb.SetText("CheckBox Text");

		flow_tests_lables.AddControl(lbl_cb);
		flow_tests_controls.AddControl(cb);

		// ProgressBar test
		Consor::CLabel lbl_prog;
		Consor::CProgressBar prog;
		lbl_prog.SetText("ProgressBar:");
		prog.SetPercent(0.275);

		flow_tests_lables.AddControl(lbl_prog);
		flow_tests_controls.AddControl(prog);

		// end test
		main_flow.AddControl(flow_tests);

		button_flow_align.ForceResize(main_flow.Size());

		Consor::CScrollContainer main_scroll(main_flow, Consor::CSize(30, 15));
		Consor::CWindowContainer window(main_scroll, "Consor Test");

		Consor::WindowSystem::RegisterWindow(window, Consor::CVector(-1, -1));
	
		while(!close)
		{
			Consor::Util::Sleep(1.0);
		}

		Consor::WindowSystem::UnregisterWindow(window);
		return 0;
	}

	Consor::WindowSystem::Close();
	return 0;
}