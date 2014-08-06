#ifndef CONSOR_SCROLLCONTAINER_H
#define CONSOR_SCROLLCONTAINER_H

#include "../Control.hpp"
#include "../Controls/HorizontalScrollbar.hpp"
#include "../Controls/VerticalScrollbar.hpp"

#include <string>

namespace Consor
{
	/// A container that allows controls to overflow.
	class ScrollContainer : public Control
	{
	protected:
		Control& _Client;
		HorizontalScrollbar _HScrollbar;
		VerticalScrollbar _VScrollbar;
		bool _ShrinkToContents;
		Size _MaxSize;
	public:
		/// \param ShrinkToContents Make the scroll container the size of it's client, unless it reaches the max size!
		ScrollContainer(Control& Client, const Size& Size, bool ShrinkToContents = false);
		virtual Size GetSize();
		virtual void OnResize(const Size& Size);
		virtual void ForceResize(const Size& Size);
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
