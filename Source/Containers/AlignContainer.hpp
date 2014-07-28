#ifndef CONSOR_ALIGNCONTAINER_H
#define CONSOR_ALIGNCONTAINER_H

#include "../Control.hpp"
#include <string>

namespace Consor
{
	/// Aligns a control on the specified axis(s).
	class AlignContainer : public Control
	{
	public:
		enum Align
		{
			Left = 0,
			Right = 1,
			Top = 0,
			Bottom = 1,
			Center = 2
		};
		enum Axis
		{
			Horizotal = 0, // TODO: Remove this line
			Horizontal = 0, // whoops, let's leave the above one for backwards compatability, for now anyway
			Vertical = 1
		};
	protected:
		Control* _pClient;
		Control* _pParent;
		bool _Sizing; // used when determining size
		Vector _ClientPos(Console::IConsoleRenderer& Renderer);
		Size _Size;
		Axis _Axis;
		Align _Align;
	public:
		AlignContainer(Control& Client, Axis Axis, Align align);
		AlignContainer(){} // this will crash, but exists only so classes can have it
		virtual Size GetSize();
		/// Used to calculate the size of the control (streches itself, so we can be placed in the center).
		virtual Size GetSize(Console::IConsoleRenderer& Renderer);
		virtual void OnResize(const Size& Size);
		virtual void ForceResize(const Size& Size);
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
		virtual bool HandleInput(Input::Key Key, Input::IInputSystem& System);
		virtual bool CanFocus();
	};
};

#endif
