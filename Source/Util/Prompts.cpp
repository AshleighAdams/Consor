#include "Prompts.hpp"
#include "Time.hpp"

#include "../Containers/WindowContainer.hpp"
#include "../Containers/FlowContainer.hpp"
#include "../Containers/AlignContainer.hpp"
#include "../Containers/ScrollContainer.hpp"

#include "../Controls/Button.hpp"
#include "../Controls/TextBox.hpp"
#include "../Controls/Label.hpp"
#include "../Controls/RadioBox.hpp"

#include "../WindowSystem.hpp"

#include <assert.h>

using namespace Consor;
using namespace std;

std::string Util::MessageBox(const std::string& Message, const std::string& Title, std::list<std::string> Buttons)
{
	assert(Buttons.size() <= 8);
	string ret = "";


	FlowContainer main_buttons(FlowContainer::FlowAxis::Horizontal, 1);

	list<tuple<Button*, string>> CreatedButtons;

	for(const string& btn : Buttons)
	{
		Button* pbtn = new Button;
		pbtn->SetText(btn);

		pbtn->Click += bind([&](string buttontext)
		{
			ret = buttontext;
		}, btn);

		main_buttons.AddControl(*pbtn);
	}

	AlignContainer align_buttons(main_buttons, AlignContainer::Axis::Horizotal, AlignContainer::Align::Center);
	Label msg;
	msg.SetText(Message);
	msg.ForceResize(Size(40, 1));

	FlowContainer main_flow(FlowContainer::FlowAxis::Vertical, 0.0);
	main_flow.AddControl(msg);
	main_flow.AddControl(align_buttons);

	align_buttons.ForceResize(main_flow.GetSize());
		
	
	while(ret.length() == 0)
	{
		Control* main = &main_flow;
		ScrollContainer main_scroll(main_flow, Size());
		
		Size flow_size = main_flow.GetSize();
		Size our_size = WindowSystem::Renderer().GetSize();
		
		if(flow_size.Height + 2 >= our_size.Height || flow_size.Width + 2 >= our_size.Width) /* 2 = window border */
		{
			main = &main_scroll; // scrolling is required!
			Size maxsize = our_size - Size(2, 2);
			Size minsize = flow_size + Size(2, 2);
			
			Size compact = Size( min(maxsize.Width, minsize.Width), min(maxsize.Height, minsize.Height) );
			main_scroll.ForceResize(compact);
			cerr << "Using scroll: size at " << compact << "\n";
		}
		
		WindowContainer window(*main, Title);
		WindowSystem::RegisterWindow(window, Vector(-1, -1));
		
		while(ret.length() == 0 && WindowSystem::Renderer().GetSize() == our_size)
			Util::Sleep(0.1);
		
		WindowSystem::UnregisterWindow(window);
	}
	
	

	for(tuple<Button*, string> tup : CreatedButtons)
		delete get<0>(tup);
	
	return ret;
}

std::string Util::ChoiceList(const std::string& Message, const std::string& Title, std::list<std::string> Choices)
{
	bool running = true;
	bool cancled = false;

	RadioBox radio;

	for(const string& choice : Choices)
		radio.AddChoice(choice);
	

	FlowContainer flow_buttons(FlowContainer::FlowAxis::Horizontal, 1.0);
	Label msg;
	msg.SetText(Message);
	msg.ForceResize(Size(40, 1));

	Button ok,cancel;
	ok.SetText("OK");
	cancel.SetText("Cancel");

	ok.Click += [&]()
	{
		if(radio.GetChoice() == "")
			return;

		running = false;
	};

	cancel.Click += [&]()
	{
		cancled = true;
		running = false;
	};

	flow_buttons.AddControl(ok);
	flow_buttons.AddControl(cancel);

	AlignContainer align_buttons(flow_buttons, AlignContainer::Axis::Horizotal, AlignContainer::Align::Center);


	FlowContainer main_flow(FlowContainer::FlowAxis::Vertical, 0);
	main_flow.AddControl(msg);
	main_flow.AddControl(radio);
	main_flow.AddControl(align_buttons);

	align_buttons.ForceResize(main_flow.GetSize());
	
	while(running)
	{
		Control* main = &main_flow;
		ScrollContainer main_scroll(main_flow, Size());
		
		Size flow_size = main_flow.GetSize();
		Size our_size = WindowSystem::Renderer().GetSize();
		
		if(flow_size.Height + 2 >= our_size.Height || flow_size.Width + 2 >= our_size.Width) /* 2 = window border */
		{
			main = &main_scroll; // scrolling is required!
			Size maxsize = our_size - Size(2, 2);
			Size minsize = flow_size + Size(2, 2);
			
			Size compact = Size( min(maxsize.Width, minsize.Width), min(maxsize.Height, minsize.Height) );
			main_scroll.ForceResize(compact);
			cerr << "Using scroll: size at " << compact << "\n";
		}
		
		WindowContainer window(*main, Title);
		WindowSystem::RegisterWindow(window, Vector(-1, -1));
		
		while(running && WindowSystem::Renderer().GetSize() == our_size)
			Util::Sleep(0.1);
		
		WindowSystem::UnregisterWindow(window);
	}
	
	return cancled ? "" : radio.GetChoice();
}

std::string Util::InputBox(const std::string& Message, const std::string& Title)
{
	bool ret = false;

	FlowContainer main_buttons(FlowContainer::FlowAxis::Horizontal, 1.0);
	Button ok;
	ok.SetText("OK");
	main_buttons.AddControl(ok);

	AlignContainer align_buttons(main_buttons, AlignContainer::Axis::Horizotal, AlignContainer::Align::Center);
	Label msg;
	msg.SetText(Message);
	msg.ForceResize(Size(40, 1));

	TextBox tb;
	

	FlowContainer main_flow(FlowContainer::FlowAxis::Vertical, 0.0);
	main_flow.AddControl(msg);
	main_flow.AddControl(tb);
	main_flow.AddControl(align_buttons);

	align_buttons.ForceResize(main_flow.GetSize());
	WindowContainer window(main_flow, Title);
	WindowSystem::RegisterWindow(window, Vector(-1, -1));
	
	ok.Click += [&]()
	{
		ret = true;
	};

	while(!ret)
	{
		Util::Sleep(0.1);
	}

	WindowSystem::UnregisterWindow(window);
	return tb.GetText();
}
