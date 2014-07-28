#ifndef GRAPHCONTROL_H
#define GRAPHCONTROL_H

#include "../Control.hpp"
#include "ProgressBar.hpp"
#include "Label.hpp"
#include "VerticalProgressBar.hpp"

#include "../Containers/FlowContainer.hpp"
#include "../Containers/AlignContainer.hpp"

#include "../Util/Hooks.hpp"

#include <list>
#include <functional>

namespace Consor
{
	class Graph : public Control
	{
	protected:
		FlowContainer _FlowHorz, _FlowVert, _FlowGraphs;
		std::list<Control*> _ToDelete;
		double _Height;
		Label _XLabel;
		Label _YLabel;
		AlignContainer _XLableAlign, _YLableAlign;
		std::function<void(double, size_t)> _Click;
	public:
		/// Fired when the user clicked a bar.  Parameters are X and Y.
		Consor::Hook<size_t, double> Click;
		Graph(double Height);
		~Graph();
		virtual Size GetSize();
		virtual void Draw(Console::IConsoleRenderer& Renderer, bool HasFocus, const ISkin& Skin);
		virtual bool HandleInput(Input::Key Key, Input::IInputSystem& System);
		virtual bool CanFocus();
		virtual VerticalProgressBar& AddBar(double Value);
		
		virtual void SetXLable(const std::string& Text);
		virtual void SetYLable(const std::string& Text);
		virtual void AddXAxisNotch(const std::string&, double Height);


	};
}

#endif
