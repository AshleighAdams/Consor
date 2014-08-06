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
	WindowContainer* pWindow = nullptr;
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
			pWindow->Close();
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
	
	// The scroll, for overflow
	Size maxsize = WindowSystem::Renderer().GetSize() - Size(2, 2);
	ScrollContainer main_scroll(main_flow, maxsize, true);
	
	WindowContainer window(main_scroll, Title);
	pWindow = &window;
	
	window.Show();
	
	for(tuple<Button*, string> tup : CreatedButtons)
		delete get<0>(tup);
	
	return ret;
}

std::string Util::ChoiceList(const std::string& Message, const std::string& Title, std::list<std::string> Choices)
{
	WindowContainer* pWindow = nullptr;
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

		pWindow->Close();
	};

	cancel.Click += [&]()
	{
		cancled = true;
		pWindow->Close();
	};

	flow_buttons.AddControl(ok);
	flow_buttons.AddControl(cancel);

	AlignContainer align_buttons(flow_buttons, AlignContainer::Axis::Horizotal, AlignContainer::Align::Center);


	FlowContainer main_flow(FlowContainer::FlowAxis::Vertical, 0);
	main_flow.AddControl(msg);
	main_flow.AddControl(radio);
	main_flow.AddControl(align_buttons);

	align_buttons.ForceResize(main_flow.GetSize());
	
	Size maxsize = WindowSystem::Renderer().GetSize() - Size(2, 2); // remove 2 for windows border
	ScrollContainer main_scroll(main_flow, maxsize, true); // shrink it if we can
	
	WindowContainer window(main_scroll, Title);
	pWindow = &window;
	
	window.Show();
	return cancled ? "" : radio.GetChoice();
}

std::string Util::InputBox(const std::string& Message, const std::string& Title)
{
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
	
	ok.Click += [&]()
	{
		window.Close();
	};
	
	window.Show();
	return tb.GetText();
}
