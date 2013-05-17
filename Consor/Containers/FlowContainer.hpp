#ifndef CONSOR_FLOWCONTAINER_H
#define CONSOR_FLOWCONTAINER_H

#include "../Control.hpp"
#include <string>
#include <list>

namespace Consor
{
	

	class FlowContainer : public CControl
	{
	public:
		enum FlowAxis
		{
			Horizontal,
			Vertical
		};
	protected:
		std::list<CControl*> _Controls;
		size_t _Focused;
		double _Seperation;
		FlowAxis _Axis;
		size_t _Focusable();
		CControl* _GetFocused();
	public:
		FlowContainer(FlowAxis axis, double Sepperation);
		virtual Size GetSize();
		virtual void OnResize(const Size& Size);
		virtual void ForceResize(const Size& Size);
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
		virtual bool HandleInput(Input::Key Key, Input::IInputSystem& System);
		virtual bool CanFocus();

		// Size is length along the FlowContainer's axis (width, if vertical), 0 to not resize control
		virtual void AddControl(CControl& Control, double Size = 0);
	};
};

#endif