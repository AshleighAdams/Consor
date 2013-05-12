#ifndef CONTROLS_CHECKBOX_H
#define CONTROLS_CHECKBOX_H

#include "Control.hpp"
#include "Label.hpp"

#include <string>

namespace Consor
{
	class CCheckBox : public CControl
	{
	protected:
		CLabel m_Label;
		bool m_Checked;
	public:
		CCheckBox();
		void SetText(std::string Text);
		bool Checked();
		void SetChecked(bool Value);
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
		virtual CSize Size();
		virtual void ForceResize(const CSize& Size);
		virtual void OnResize(const CSize& Size);
		virtual bool HandleInput(Input::Key Key);
		virtual bool CanFocus();
	};
}

#endif