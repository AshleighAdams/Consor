#ifndef GRAPHCONTROL_H
#define GRAPHCONTROL_H

#include "../Control.hpp"
#include "ProgressBar.hpp"
#include "Label.hpp"

#include "../Containers/FlowContainer.hpp"
#include "../Containers/AlignContainer.hpp"

#include "../Util/Hooks.hpp"

#include <list>
#include <functional>

namespace Consor
{
	class CGraph : public CControl
	{
	protected:
		CFlowContainer m_FlowHorz, m_FlowVert, m_FlowGraphs;
		std::list<CControl*> m_ToDelete;
		double m_Height;
		CLabel m_XLabel;
		CLabel m_YLabel;
		CAlignContainer m_XLableAlign, m_YLableAlign;
		std::function<void(double, size_t)> _Click;
	public:
		Consor::CHook<double, size_t> Click;
		CGraph(double Height);
		~CGraph();
		virtual CSize Size();
		virtual void Draw(Console::IConsoleRenderer& Renderer, bool HasFocus, const ISkin& Skin);
		virtual bool HandleInput(Input::Key Key, Input::IInputSystem& System);
		virtual bool CanFocus();
		virtual void AddBar(double Value);

		virtual void SetXLable(const std::string& Text);
		virtual void SetYLable(const std::string& Text);
		virtual void AddXAxisNotch(const std::string&, double Height);


	};
}

#endif