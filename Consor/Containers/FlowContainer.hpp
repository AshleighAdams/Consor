#ifndef CONSOR_FLOWCONTAINER_H
#define CONSOR_FLOWCONTAINER_H

#include "Control.hpp"
#include <string>
#include <list>

namespace Consor
{
	class CFlowContainer : public CControl
	{
	protected:
		std::list<CControl*> m_Controls;
		std::list<CControl*>::iterator m_ControlItterator;
	public:
		CFlowContainer();
		virtual CSize Size();
		virtual void OnResize(const CSize& Size);
		virtual void ForceResize(const CSize& Size);
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
		virtual bool HandleInput(int Key);
		virtual bool CanFocus();

		virtual void AddControl(CControl& Control);
	};
};

#endif