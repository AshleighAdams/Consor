#ifndef CONTROL_H
#define CONTROL_H

#include "Units.hpp"
#include "Skin.hpp"
#include "ConsoleRenderer.hpp"
#include "InputSystem.hpp"

namespace Consor
{
	class CControl
	{
	protected:
		Consor::CSize m_Size;
	public:
		virtual ~CControl(){}
		virtual CSize Size();
		virtual void OnResize(const CSize& Size);
		virtual void ForceResize(const CSize& Size);
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
		virtual bool HandleInput(Input::Key Key);
		virtual bool CanFocus();
	};
};

#endif