#include "Graph.hpp"
#include "VerticalProgressBar.hpp"

using namespace Consor;

CGraph::CGraph(double Height) : 
	m_Flow(Consor::CFlowContainer::FlowAxis::Horizontal, 0),
	m_FlowVert(Consor::CFlowContainer::FlowAxis::Vertical, 0),
	m_Height(Height),
	m_YLableAlign(m_YLabel, CSize(0, Height), CAlignContainer::Axis::Vertical, CAlignContainer::Align::Center),
	m_XLableAlign(m_XLabel, CSize(0, 1), CAlignContainer::Axis::Horizotal, CAlignContainer::Align::Center)
{
	m_Flow.AddControl(m_YLableAlign);
	m_FlowVert.AddControl(m_Flow);
	m_FlowVert.AddControl(m_XLableAlign);

	_Click = [&](double a, size_t b)
	{
		this->Click(a, b);
	};
}

CGraph::~CGraph()
{
	for(CControl* c : m_ToDelete)
		delete c;
}

CSize CGraph::Size()
{
	return m_FlowVert.Size();
}

void CGraph::Draw(Console::IConsoleRenderer& Renderer, bool HasFocus, const ISkin& Skin)
{
	m_FlowVert.Draw(Renderer, HasFocus, Skin);
}

bool CGraph::CanFocus()
{
	return true;
}


void CGraph::AddBar(double Value)
{
	CVerticalProgressBar* pb = new CVerticalProgressBar();
	pb->ForceResize(CSize(1, m_Height));
	pb->SetPercent(Value);
	pb->SetGraphMode(m_ToDelete.size());

	pb->Click += _Click;

	m_ToDelete.push_back(pb);
	m_Flow.AddControl(*pb);
	m_XLableAlign.ForceResize(m_Flow.Size());
}

void CGraph::SetXLable(const std::string& Text)
{
	m_XLabel.SetText(Text);
}

void CGraph::SetYLable(const std::string& Text)
{
	m_YLabel.SetText(Text);
}

void CGraph::AddXAxisNotch(const std::string& Text, double Height)
{

}

bool CGraph::HandleInput(Input::Key Key, Input::IInputSystem& System)
{
	return m_FlowVert.HandleInput(Key, System);
}