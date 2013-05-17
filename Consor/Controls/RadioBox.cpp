#include "RadioBox.hpp"

using namespace Consor;

RadioBox::RadioBox() : _FlowContainer(FlowContainer::FlowAxis::Vertical, 0)
{

}

RadioBox::~RadioBox()
{
	for(std::tuple<CheckBox*, std::string> cb : _Checkboxes)
		delete std::get<0>(cb);
}


void RadioBox::AddChoice(const std::string& Choice)
{
	CheckBox* cb = new CheckBox();
	cb->SetText(Choice);
	
	_FlowContainer.AddControl(*cb);
	_Checkboxes.push_back(std::make_tuple(cb, Choice));
}

std::string RadioBox::GetChoice()
{
	for(std::tuple<CheckBox*, std::string> cb : _Checkboxes)
		if(std::get<0>(cb)->Checked())
			return std::get<1>(cb);

	return "";
}

void RadioBox::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	_FlowContainer.Draw(Renderer, HasFocus, Skin);
}

Size RadioBox::GetSize()
{
	return _FlowContainer.GetSize();
}

void RadioBox::OnResize(const Size& Size)
{
}

void RadioBox::ForceResize(const Size& Size)
{
}

bool RadioBox::HandleInput(Input::Key Key, Input::IInputSystem& System)
{
	// if enter or space, don't send input just yet, set all checkboxes to unchecked.
	// this also prevents a checked checkbox from being unchecked
	if(Key == Input::Key::Enter || Key == Input::Key::Space)
	{
		for(std::tuple<CheckBox*, std::string> cb : _Checkboxes)
			std::get<0>(cb)->SetChecked(false);
	}

	// ok, now send it
	return _FlowContainer.HandleInput(Key, System);
}

bool RadioBox::CanFocus()
{
	return _FlowContainer.CanFocus();
}