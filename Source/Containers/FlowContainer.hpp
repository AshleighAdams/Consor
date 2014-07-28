#ifndef CONSOR_FLOWCONTAINER_H
#define CONSOR_FLOWCONTAINER_H

#include "../Control.hpp"
#include <string>
#include <list>

namespace Consor
{
	/// 
	class FlowContainer : public Control
	{
	public:
		enum FlowAxis
		{
			Horizontal,
			Vertical
		};
	protected:
		std::list<Control*> _Controls;
		size_t _Focused;
		double _Seperation;
		FlowAxis _Axis;
		size_t _Focusable();
		Control* _GetFocused();
	public:
		/// \param axis The direction the controls flow.
		/// \param Separation The amount of separation between each control.
		FlowContainer(FlowAxis axis, double Separation);
		virtual Size GetSize();
		virtual void OnResize(const Size& Size);
		virtual void ForceResize(const Size& Size);
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
		virtual bool HandleInput(Input::Key Key, Input::IInputSystem& System);
		virtual bool CanFocus();

		// Size is length along the FlowContainer's axis (width, if vertical), 0 to not resize control
		virtual void AddControl(Control& Control, double Size = 0);
	};
};

#endif
