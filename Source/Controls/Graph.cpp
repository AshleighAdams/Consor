#include "Graph.hpp"
#include "VerticalProgressBar.hpp"

using namespace Consor;

Graph::Graph(double Height) : 
	_FlowHorz(Consor::FlowContainer::FlowAxis::Horizontal, 0),
	_FlowVert(Consor::FlowContainer::FlowAxis::Vertical, 0),
	_FlowGraphs(Consor::FlowContainer::FlowAxis::Horizontal, 0),
	_Height(Height),
	_XLableAlign(_XLabel, AlignContainer::Axis::Horizotal, AlignContainer::Align::Center),
	_YLableAlign(_YLabel, AlignContainer::Axis::Vertical, AlignContainer::Align::Center)
{
	_FlowHorz.AddControl(_YLableAlign);
	_FlowHorz.AddControl(_FlowVert);
	_FlowVert.AddControl(_FlowGraphs);
	_FlowVert.AddControl(_XLableAlign);

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
	_XLableAlign.ForceResize(_FlowGraphs.GetSize());

	return *pb;
}

void Graph::SetXLable(const std::string& Text)
{
	_XLabel.SetText(Text);
}

void Graph::SetYLable(const std::string& Text)
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
