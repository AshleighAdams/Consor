#ifndef CONTROLS_BUTTON_H
#define CONTROLS_BUTTON_H

#include "../Control.hpp"
#include "Label.hpp"
#include "../Containers/AlignContainer.hpp"
#include "../Util/Hooks.hpp"

#include <string>

namespace Consor
{
	class CButton : public CControl
	{
	protected:
		CAlignContainer m_VAlign;
		CAlignContainer m_HAlign;
		CLabel m_Label;
	public:
		CHook<> Click;

		CButton();
		void SetText(std::string Text);
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
		virtual void ForceResize(const CSize& Size);
		virtual void OnResize(const CSize& Size);
		virtual bool HandleInput(Input::Key Key, Input::IInputSystem& System);
		virtual bool CanFocus();
	};
}

#endif