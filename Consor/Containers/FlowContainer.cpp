#include "FlowContainer.hpp"

using namespace Consor;
using namespace std;

CFlowContainer::CFlowContainer(CFlowContainer::FlowAxis Axis, double Seperation)
{
	m_Axis = Axis;
	m_Seperation = Seperation;
	m_Focused = 0;
}

size_t CFlowContainer::m_Focusable()
{
	size_t ret = 0;

	for(CControl* c : m_Controls)
		if(c->CanFocus())
			ret++;

	return ret;
}

CControl* CFlowContainer::m_GetFocused()
{
	if(m_Focusable() == 0)
		return nullptr;

	size_t cur = 0;

	for(CControl* c : m_Controls)
		if(c->CanFocus())
		{
			if(m_Focused == cur)
				return c;
			cur++;
		}

	return nullptr;
}

CSize CFlowContainer::Size()
{
	CSize ret;

	double span = 0.0;

	for(CControl* ctrl : m_Controls)
	{
		CSize size = ctrl->Size();

		if(m_Axis == CFlowContainer::FlowAxis::Vertical)
		{
			if(size.Width > ret.Width)
				ret.Width = size.Width;

			span += size.Height + m_Seperation;
		}
		else
		{
			if(size.Height > ret.Height)
				ret.Height = size.Height;

			span += size.Width + m_Seperation;
		}
	}

	span -= m_Seperation;

	if(m_Axis == CFlowContainer::FlowAxis::Vertical)
		ret.Height = span;
	else
		ret.Width = span;

	return ret;
}

void CFlowContainer::OnResize(const CSize& Size)
{
}

void CFlowContainer::ForceResize(const CSize& Size)
{
	throw std::exception("`CFlowContainer' can't be resized, please use a `CScrollContainer'");
}

void CFlowContainer::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	CVector pos = CVector(0, 0);
	CControl* pFocused = m_GetFocused();

	for(CControl* ctrl : m_Controls)
	{
		CSize ctrl_size = ctrl->Size();

		Renderer.PushRenderBounds(pos, ctrl_size);
		ctrl->Draw(Renderer, HasFocus && ctrl == pFocused, Skin);
		Renderer.PopRenderBounds();
		
		if(m_Axis == CFlowContainer::FlowAxis::Vertical)
			pos.Y += ctrl_size.Height + m_Seperation;
		else
			pos.X += ctrl_size.Width + m_Seperation;
	}
}

bool CFlowContainer::HandleInput(Input::Key Key)
{
	CControl* pFocused = m_GetFocused();

	if(pFocused && pFocused->HandleInput(Key))
		return true;

	if(Key == Input::Key::Up || Key == Input::Key::Left)
	{
		if(m_Focused <= 0)
			return false;
		m_Focused--;
		return true;
	}
	else if(Key == Input::Key::Down || Key == Input::Key::Right)
	{
		size_t focusable = m_Focusable();

		if(m_Focused >= focusable - 1)
			return false;
		m_Focused++;
		return true;
	}
}

bool CFlowContainer::CanFocus()
{
	for(CControl* Control : m_Controls) // return true if one of hour controls can obtain focus
		if(Control->CanFocus())
			return true;
}

void CFlowContainer::AddControl(CControl& Control)
{
	m_Controls.push_back(&Control);
}