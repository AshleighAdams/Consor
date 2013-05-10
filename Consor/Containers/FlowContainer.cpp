#include "FlowContainer.hpp"

using namespace Consor;
using namespace std;

CFlowContainer::CFlowContainer()
{
	m_ControlItterator = m_Controls.begin();
}

CSize CFlowContainer::Size()
{
	CSize ret;

	for(CControl* ctrl : m_Controls)
	{
		CSize size = ctrl->Size();
		if(size.Width > ret.Width)
			ret.Width = size.Width;

		ret.Height += size.Height;
	}

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

	for(CControl* ctrl : m_Controls)
	{
		CSize ctrl_size = ctrl->Size();

		Renderer.PushRenderBounds(pos, ctrl_size);
		ctrl->Draw(Renderer, false, Skin);
		Renderer.PopRenderBounds();
		
		pos.Y += ctrl_size.Height;
	}
}

bool CFlowContainer::HandleInput(int Key)
{
	throw std::exception("not implimented");
	return false;
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