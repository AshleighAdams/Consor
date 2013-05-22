#include <iostream>
#include <thread>
#include <mutex>

#ifdef _WIN32
	#include "WindowsConsoleRenderer.hpp"
	#include "WindowsInputSystem.hpp"
	typedef Consor::Console::WindowsConsoleRenderer PlatformRenderer;
	typedef Consor::Input::WindowsInputSystem PlatformInputSystem;
#else
	#include "ANSIConsoleRenderer.hpp"
	#include "LinuxInputSystem.hpp"
	typedef Consor::Console::ANSIConsoleRenderer PlatformRenderer;
	typedef Consor::Input::LinuxInputSystem PlatformInputSystem;
#endif

#include "Util/StringUtils.hpp"
#include "Util/Time.hpp"
#include "Util/FrequencyController.hpp"
#include "Util/Math.hpp"
#include "Util/Prompts.hpp"



#include "Controls/Label.hpp"
#include "Controls/Button.hpp"
#include "Controls/TextBox.hpp"
#include "Controls/PasswordBox.hpp"
#include "Controls/CheckBox.hpp"
#include "Controls/ProgressBar.hpp"
#include "Controls/VerticalProgressBar.hpp"
#include "Controls/HorizontalScrollbar.hpp"
#include "Controls/VerticalScrollbar.hpp"

#include "Containers/WindowContainer.hpp"
#include "Containers/BorderContainer.hpp"
#include "Containers/FlowContainer.hpp"
#include "Containers/AlignContainer.hpp"
#include "Containers/ScrollContainer.hpp"

#include "WindowSystem.hpp"

using namespace std;

// this skin is usefull for debugging in Linux, as the colours don't become hard to read when the debugger is invoked
class MonoSkin : public Consor::DefaultSkin
{
public:
	MonoSkin(Consor::Console::IConsoleRenderer& Renderer)
	{
		WindowLeft = WindowRight = ' ';
		_ColourPos = 0;

		Foreground = RequestColour(Renderer, Consor::Colour(1, 1, 1));
		ForegroundShine = Foreground;
		Background = RequestColour(Renderer, Consor::Colour(0, 0, 0));
		AlternateBackground = Background;
		FocusColour = RequestColour(Renderer, Consor::Colour(0.5, 0.5, 0.5));
		ProgressPercent = Consor::Colour(0, 0, 0, 0);
		ProgressForeground = Foreground;
		CanvasColour = Background;
	}
};

class CSaneSkin : public Consor::DefaultSkin
{
public:
	CSaneSkin(Consor::Console::IConsoleRenderer& Renderer)
	{
		WindowLeft = ' ';
		WindowRight = ' ';

		_ColourPos = 0;
		Foreground = RequestColour(Renderer, Consor::Colour(0, 0, 0));
		ForegroundShine = RequestColour(Renderer, Consor::Colour(1, 1, 1));
		Background = RequestColour(Renderer, Consor::Colour(0.7, 0.7, 0.7));
		AlternateBackground = RequestColour(Renderer, Consor::Colour(0.3, 0.3, 0.3));
		FocusColour = RequestColour(Renderer, Consor::Colour(0, 0, 1));
		ProgressPercent = Consor::Colour(0, 0, 0, 0);
		ProgressForeground = RequestColour(Renderer, Consor::Colour(0, 1, 0));
		CanvasColour = RequestColour(Renderer, Consor::Colour(0.392, 0.548, 0.929));
	};
};

//#include <codecvt>
#include <locale>

int main(int count, char** values)
{
	Consor::WindowSystem::Setup(new PlatformRenderer(),
		new PlatformInputSystem());

	Consor::WindowSystem::SetSkin<MonoSkin>();

	atexit([]()
	{
		// safley close the window system at exit
		Consor::WindowSystem::Close();
	});

	Consor::WindowSystem::RegisterHotKey(nullptr, Consor::Input::Key::F2, false, false, []()
	{
		cerr << "CALLBACK";
	});

	list<string> skins;
	skins.push_back("Default");
	skins.push_back("Mono");
	skins.push_back("Sane");
	skins.push_back("Hacker");
	string selected = Consor::Util::ChoiceList("Please select a skin.", "Select Skin", skins);

	if(selected == "Mono")
	{
		Consor::WindowSystem::SetSkin<MonoSkin>();
	}
	if(selected == "Sane")
	{
		Consor::WindowSystem::SetSkin<CSaneSkin>();
	}
	else if(selected == "Hacker")
	{
		Consor::WindowSystem::SetSkin<Consor::HackerSkin>();
	}

	// the login box (not for real, just a test)
	{
		using namespace Consor;

		FlowContainer flow_table(FlowContainer::FlowAxis::Horizontal, 4);
		FlowContainer flow_table_col1(FlowContainer::FlowAxis::Vertical, 0);
		FlowContainer flow_table_col2(FlowContainer::FlowAxis::Vertical, 0);

		flow_table.AddControl(flow_table_col1);
		flow_table.AddControl(flow_table_col2);

		Label lbl_un, lbl_pw;
		lbl_un.SetText("Username:");
		lbl_pw.SetText("Password:");
		
		flow_table_col1.AddControl(lbl_un);
		flow_table_col1.AddControl(lbl_pw);

		TextBox un; PasswordBox pw;
		un.SetText("test");

		flow_table_col2.AddControl(un);
		flow_table_col2.AddControl(pw);

		FlowContainer flow_buttons(FlowContainer::FlowAxis::Horizontal, 1.0);
		Button ok, cancel;
		ok.SetText("Login");
		cancel.SetText("Exit");
		flow_buttons.AddControl(ok);
		flow_buttons.AddControl(cancel);

		AlignContainer align_flow_buttons(flow_buttons, Size(), AlignContainer::Axis::Horizotal, AlignContainer::Align::Center);

		Label msg;
		AlignContainer align_msg(msg, Size(), AlignContainer::Axis::Horizotal, AlignContainer::Align::Center);

		FlowContainer flow_main(FlowContainer::FlowAxis::Vertical, 0);
		flow_main.AddControl(flow_table);
		flow_main.AddControl(align_msg);
		flow_main.AddControl(align_flow_buttons);

		align_flow_buttons.ForceResize(flow_main.GetSize());
		align_msg.ForceResize(flow_main.GetSize());
		WindowContainer window(flow_main, "Login");
		
		bool loggedin = false;

		ok.Click += [&]()
		{
			if(un.GetText() == "test" && pw.GetPassword() == "test")
				loggedin = true;
			else 
			{
				std::list<string> btns;
				btns.push_back("OK");
				Util::MessageBox("The username or password you supplied is invalid!", "Error", btns);
				pw.SetText("");
			}
		};

		bool close = false;
		cancel.Click += [&]()
		{
			close = true;
		};
		
		WindowSystem::RegisterWindow(window, Vector(-1, -1));

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

		Label consoletext;
		FlowContainer flow(FlowContainer::FlowAxis::Vertical, 0);
		flow.AddControl(consoletext);

		ScrollContainer scroll(flow, Size(40, 8));

		string total = "";
		bool notscrolled = true;
		auto addtext = [&](const std::string& msg)
		{
			WindowSystem::Lock(); // we're messing with something that the main draw thread will be too
			total += msg + "\n";
			consoletext.SetText(total);
			consoletext.ForceResize(scroll.GetSize() - Size(1, 1));
			
			if(notscrolled && scroll.ScrollDown())
				notscrolled = false;
			WindowSystem::Unlock();
		};

		WindowContainer window(scroll, "Logging in");

		WindowSystem::RegisterWindow(window, Vector(-1, -1));

		addtext("commencing the ScrollContainer auto scrolling test..."); Util::Sleep(0.5);
		addtext("connecting to login server..."); Util::Sleep(0.5);
		addtext("logging in..."); Util::Sleep(2);

		string reasons[] = {"information", "datapack", "members", "updates", "earth", "universe", "lorem ipsum", "oxford dictionary"};

		for(unsigned int i = 0; i < (sizeof reasons / sizeof reasons[0]); i++)
		{
			addtext(Util::FormatString("downloading \"%\"...", reasons[i])); Util::Sleep(0.75);
		}

		addtext("launching..."); Util::Sleep(1);
		addtext("loading UI..."); Util::Sleep(1);

		ProgressBar progbar;
		progbar.ForceResize(Size(flow.GetSize().Width - 2, 1));

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
		Consor::Label msg;
		//msg.SetText("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas tempor metus sed ligula tempor tincidunt. Nullam quis condimentum augue. Nulla varius, nunc venenatis molestie egestas, neque lorem bibendum dui, vitae placerat magna nunc at nulla. In ultricies lectus quis purus bibendum eget ullamcorper metus tempus. Phasellus pulvinar, est sit amet auctor tempus, turpis nisl cursus mauris, vitae hendrerit felis tellus eu turpis. Vestibulum id leo sed magna vehicula aliquet. Fusce viverra auctor augue ut rutrum. Quisque quis nisl non turpis sollicitudin rutrum sit amet eget libero. Donec pretium egestas ante, eu aliquam mi porttitor quis.");
	
		msg.SetText(Consor::Util::FormatString("The current renderer is %, version string \"%\".", Consor::WindowSystem::RendererName(), Consor::WindowSystem::RendererVersionString()));

		msg.ForceResize(Consor::Size(36, 1));
		Consor::ScrollContainer msg_scroll(msg, Consor::Size(-1, 10));
	
		Consor::FlowContainer button_flow(Consor::FlowContainer::FlowAxis::Horizontal, 1);
		Consor::Button exit;
		exit.SetText("Exit");

		button_flow.AddControl(exit);

		bool close = false;
		exit.Click += [&]()
		{
			close = true;
		};

		Consor::AlignContainer button_flow_align(button_flow, Consor::Size(),
			Consor::AlignContainer::Axis::Horizotal, Consor::AlignContainer::Align::Center);

		Consor::FlowContainer main_flow(Consor::FlowContainer::FlowAxis::Vertical, 1);
		main_flow.AddControl(msg);
		main_flow.AddControl(button_flow_align);

	

		Consor::FlowContainer flow_tests(Consor::FlowContainer::FlowAxis::Horizontal, 4.0);
		Consor::FlowContainer flow_tests_lables(Consor::FlowContainer::FlowAxis::Vertical, 0.0);
		Consor::FlowContainer flow_tests_controls(Consor::FlowContainer::FlowAxis::Vertical, 0.0);

		flow_tests.AddControl(flow_tests_lables);
		flow_tests.AddControl(flow_tests_controls);

		// the tests subjects
	
		Consor::Label lbl_left, lbl_right;
		lbl_left.SetText("Control Name");
		lbl_right.SetText("Control");

		Consor::Label lbl_left_empty, lbl_right_empty;
		lbl_left_empty.SetText("");
		lbl_right_empty.SetText("");

		flow_tests_lables.AddControl(lbl_left);
		flow_tests_lables.AddControl(lbl_left_empty);
		flow_tests_controls.AddControl(lbl_right);
		flow_tests_controls.AddControl(lbl_right_empty);


		// Textbox test
		Consor::Label lbl_tb;
		Consor::TextBox tb;
		lbl_tb.SetText("TextBox:");
		tb.ForceResize(Consor::Size(20, 1));

		flow_tests_lables.AddControl(lbl_tb);
		flow_tests_controls.AddControl(tb);

		// Button test
		Consor::Label lbl_btn;
		Consor::Button btn;
		lbl_btn.SetText("Button:");
		btn.SetText("Button");
		btn.Click += [&]()
		{
			std::list<std::string> buttons; // why no {"OK"} windows?
			buttons.push_back("OK");
			Consor::Util::MessageBox("You pressed the button", "Button Pressed", buttons);
		};

		flow_tests_lables.AddControl(lbl_btn);
		flow_tests_controls.AddControl(btn);

		//HScrollbar test
		Consor::Label lbl_scroll;
		Consor::HorizontalScrollbar scroll;
		lbl_scroll.SetText("HScrollbar:");
		scroll.ForceResize(Consor::Size(20, 1));
		scroll.SetPercent(0.5);

		flow_tests_lables.AddControl(lbl_scroll);
		flow_tests_controls.AddControl(scroll);

		// PasswordBox test
		Consor::Label lbl_pb;
		Consor::PasswordBox pb;
		lbl_pb.SetText("PasswordBox:");
		pb.ForceResize(Consor::Size(20, 1));

		flow_tests_lables.AddControl(lbl_pb);
		flow_tests_controls.AddControl(pb);

		// CheckBox test
		Consor::Label lbl_cb;
		Consor::CheckBox cb;
		lbl_cb.SetText("CheckBox:");
		cb.SetText("CheckBox Text");

		flow_tests_lables.AddControl(lbl_cb);
		flow_tests_controls.AddControl(cb);

		// ProgressBar test
		Consor::Label lbl_prog;
		Consor::ProgressBar prog;
		lbl_prog.SetText("ProgressBar:");
		prog.SetPercent(0.275);

		flow_tests_lables.AddControl(lbl_prog);
		flow_tests_controls.AddControl(prog);

		// VerticalProgressBar test
		Consor::Label lbl_progv;
		Consor::VerticalProgressBar progv;
		lbl_progv.SetText("VProgressBar:");
		progv.SetPercent(0.275);

		flow_tests_lables.AddControl(lbl_progv);
		flow_tests_controls.AddControl(progv);

		// end test
		main_flow.AddControl(flow_tests);

		button_flow_align.ForceResize(main_flow.GetSize());

		Consor::ScrollContainer main_scroll(main_flow, Consor::Size(30, 15));
		Consor::WindowContainer window(main_scroll, "Consor Test");

		Consor::WindowSystem::RegisterWindow(window, Consor::Vector(1, 1));
	
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
