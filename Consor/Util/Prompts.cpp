#include "Prompts.hpp"
#include "Time.hpp"

#include "Containers/WindowContainer.hpp"
#include "Containers/FlowContainer.hpp"
#include "Containers/AlignContainer.hpp"

#include "Controls/Button.hpp"
#include "Controls/Label.hpp"
#include "Controls/RadioBox.hpp"

#include "WindowSystem.hpp"

#include <assert.h>

using namespace Consor;
using namespace std;

std::string Util::MessageBox(const std::string& Message, const std::string& Title, std::list<std::string> Buttons)
{
	assert(Buttons.size() <= 8);
	string ret = "";


	CFlowContainer main_buttons(CFlowContainer::FlowAxis::Horizontal, 0.0);
	CButton buttons[8];
	size_t pos = 0;

	for(const string& btn : Buttons)
	{
		string btnname = btn;
		buttons[pos].SetText(btnname);
		buttons[pos].Click += [&]()
		{
			ret = btnname;
		};

		main_buttons.AddControl(buttons[pos]);
		pos++;
	}

	CAlignContainer align_buttons(main_buttons, CSize(), CAlignContainer::Axis::Horizotal, CAlignContainer::Align::Center);
	CLabel msg;
	msg.SetText(Message);

	CFlowContainer main_flow(CFlowContainer::FlowAxis::Vertical, 0.0);
	main_flow.AddControl(msg);
	main_flow.AddControl(align_buttons);

	align_buttons.ForceResize(main_flow.Size());
	CWindowContainer window(main_flow, Title);
	WindowSystem::RegisterWindow(window, CVector(-1, -1));
	
	while(ret.length() == 0)
	{
		Util::Sleep(0.1);
	}

	WindowSystem::UnregisterWindow(window);
	return ret;
}

std::string Util::ChoiceList(const std::string& Message, const std::string& Title, std::list<std::string> Choices)
{
	bool running = true;
	bool cancled = false;

	CRadioBox radio;

	for(const string& choice : Choices)
		radio.AddChoice(choice);


	CFlowContainer flow_buttons(CFlowContainer::FlowAxis::Horizontal, 1.0);
	CLabel msg;
	msg.SetText(Message);
	CButton ok,cancel;
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

	CAlignContainer align_buttons(flow_buttons, CSize(), CAlignContainer::Axis::Horizotal, CAlignContainer::Align::Center);


	CFlowContainer main_flow(CFlowContainer::FlowAxis::Vertical, 0);
	main_flow.AddControl(msg);
	main_flow.AddControl(radio);
	main_flow.AddControl(align_buttons);

	align_buttons.ForceResize(main_flow.Size());
	CWindowContainer window(main_flow, Title);
	WindowSystem::RegisterWindow(window, CVector(-1, -1));

	while(running)
	{
		Util::Sleep(0.1);
	}

	WindowSystem::UnregisterWindow(window);

	return cancled ? "" : radio.GetChoice();
}

std::string Util::InputBox(const std::string& Message, const std::string& Title)
{
	return "";
}