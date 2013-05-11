#ifndef CONSOR_FLOWCONTAINER_H
#define CONSOR_FLOWCONTAINER_H

#include "Control.hpp"
#include <string>
#include <list>

namespace Consor
{
	

	class CFlowContainer : public CControl
	{
	public:
		enum FlowAxis
		{
			Horizontal,
			Vertical
		};
	protected:
		std::list<CControl*> m_Controls;
		size_t m_Focused;
		double m_Seperation;
		FlowAxis m_Axis;
		size_t m_Focusable();
		CControl* m_GetFocused();
	public:
		CFlowContainer(FlowAxis axis, double Sepperation);
		virtual CSize Size();
		virtual void OnResize(const CSize& Size);
		virtual void ForceResize(const CSize& Size);
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
		virtual bool HandleInput(Input::Key Key);
		virtual bool CanFocus();

		virtual void AddControl(CControl& Control);
	};
};

#endif