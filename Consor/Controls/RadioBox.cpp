#include "RadioBox.hpp"

using namespace Consor;

CRadioBox::CRadioBox() : m_FlowContainer(CFlowContainer::FlowAxis::Vertical, 0)
{

}

CRadioBox::~CRadioBox()
{
	for(std::tuple<CCheckBox*, std::string> cb : m_Checkboxes)
		delete std::get<0>(cb);
}


void CRadioBox::AddChoice(const std::string& Choice)
{
	CCheckBox* cb = new CCheckBox();
	cb->SetText(Choice);
	
	m_FlowContainer.AddControl(*cb);
	m_Checkboxes.push_back(std::make_tuple(cb, Choice));
}

std::string CRadioBox::GetChoice()
{
	for(std::tuple<CCheckBox*, std::string> cb : m_Checkboxes)
		if(std::get<0>(cb)->Checked())
			return std::get<1>(cb);

	return "";
}

void CRadioBox::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	m_FlowContainer.Draw(Renderer, HasFocus, Skin);
}

CSize CRadioBox::Size()
{
	return m_FlowContainer.Size();
}

void CRadioBox::OnResize(const CSize& Size)
{
}

void CRadioBox::ForceResize(const CSize& Size)
{
}

bool CRadioBox::HandleInput(Input::Key Key, Input::IInputSystem& System)
{
	// if enter or space, don't send input just yet, set all checkboxes to unchecked.
	// this also prevents a checked checkbox from being unchecked
	if(Key == Input::Key::Enter || Key == Input::Key::Space)
	{
		for(std::tuple<CCheckBox*, std::string> cb : m_Checkboxes)
			std::get<0>(cb)->SetChecked(false);
	}

	// ok, now send it
	return m_FlowContainer.HandleInput(Key, System);
}

bool CRadioBox::CanFocus()
{
	return m_FlowContainer.CanFocus();
}