#ifndef CONSOR_SCROLLCONTAINER_H
#define CONSOR_SCROLLCONTAINER_H

#include "../Control.hpp"
#include "../Controls/HorizontalScrollbar.hpp"
#include "../Controls/VerticalScrollbar.hpp"

#include <string>

namespace Consor
{
	class CScrollContainer : public CControl
	{
	protected:
		CControl* m_pClient;
		CHorizontalScrollbar m_HScrollbar;
		CVerticalScrollbar m_VScrollbar;
	public:
		CScrollContainer(CControl& Client, const CSize& Size);
		CScrollContainer(){} // this should never actually be used
		virtual CSize Size();
		virtual void OnResize(const CSize& Size);
		virtual void ForceResize(const CSize& Size);
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
		virtual bool HandleInput(Input::Key Key, Input::IInputSystem& System);
		virtual bool CanFocus();

		virtual bool ScrollLeft(size_t count = 1);
		virtual bool ScrollRight(size_t count = 1);
		virtual bool ScrollUp(size_t count = 1);
		virtual bool ScrollDown(size_t count = 1);
	};
};

#endif