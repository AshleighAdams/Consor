#ifndef CONSOR_ALIGNCONTAINER_H
#define CONSOR_ALIGNCONTAINER_H

#include "../Control.hpp"
#include <string>

namespace Consor
{
	class CAlignContainer : public CControl
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
			Horizotal,
			Vertical
		};
	protected:
		CControl* m_pClient;
		CVector m_ClientPos();
		CSize m_Size;
		Axis m_Axis;
		Align m_Align;
	public:
		CAlignContainer(CControl& Client, const CSize& Size, Axis Axis, Align align);
		CAlignContainer(){} // this will crash, but exists only so classes can have it
		virtual CSize Size();
		virtual void OnResize(const CSize& Size);
		virtual void ForceResize(const CSize& Size);
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
		virtual bool HandleInput(Input::Key Key, Input::IInputSystem& System);
		virtual bool CanFocus();
	};
};

#endif