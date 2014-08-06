#include "Graph.hpp"
#include "VerticalProgressBar.hpp"

using namespace Consor;

Graph::Graph(double Height) : 
	_FlowHorz(Consor::FlowContainer::FlowAxis::Horizontal, 0),
	_FlowVert(Consor::FlowContainer::FlowAxis::Vertical, 0),
	_FlowGraphs(Consor::FlowContainer::FlowAxis::Horizontal, 0),
	_Height(Height),
	_XLabelAlign(_XLabel, AlignContainer::Axis::Horizotal, AlignContainer::Align::Center),
	_YLabelAlign(_YLabel, AlignContainer::Axis::Vertical, AlignContainer::Align::Center)
{
	_FlowHorz.AddControl(_YLabelAlign);
	_FlowHorz.AddControl(_FlowVert);
	_FlowVert.AddControl(_FlowGraphs);
	_FlowVert.AddControl(_XLabelAlign);

	_Click = [&](double a, size_t b)
	{
		this->Click(b, a); // flipped over Y, X to X, Y
	};
}

Graph::~Graph()
{
	for(Control* c : _ToDelete)
		delete c;
}

Size Graph::GetSize()
{
	return _FlowHorz.GetSize();
}

void Graph::Draw(Console::IConsoleRenderer& Renderer, bool HasFocus, const ISkin& Skin)
{
	_FlowHorz.Draw(Renderer, HasFocus, Skin);
}

bool Graph::CanFocus()
{
	return true;
}


VerticalProgressBar& Graph::AddBar(double Value)
{
	VerticalProgressBar* pb = new VerticalProgressBar();
	pb->ForceResize(Size(1, _Height));
	pb->SetPercent(Value);
	pb->SetGraphMode(_ToDelete.size());

	pb->Click += _Click;

	_ToDelete.push_back(pb);
	_FlowGraphs.AddControl(*pb);
	_XLabelAlign.ForceResize(_FlowGraphs.GetSize());

	return *pb;
}

void Graph::SetXLabel(const std::string& Text)
{
	_XLabel.SetText(Text);
}

void Graph::SetYLabel(const std::string& Text)
{
	_YLabel.SetText(Text);
}

void Graph::AddXAxisNotch(const std::string& Text, double Height)
{

}

bool Graph::HandleInput(Input::Key Key, Input::IInputSystem& System)
{
	return _FlowHorz.HandleInput(Key, System);
}
