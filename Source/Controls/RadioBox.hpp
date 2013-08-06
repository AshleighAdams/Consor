#ifndef CONTROLS_RADIOBOX_H
#define CONTROLS_RADIOBOX_H

#include "../Control.hpp"
#include "CheckBox.hpp"
#include "../Containers/FlowContainer.hpp"

#include <string>
#include <tuple>

namespace Consor
{
	class RadioBox : public Control
	{
	protected:
		std::list<std::tuple<CheckBox*, std::string>> _Checkboxes;
		FlowContainer _FlowContainer;
	public:
		RadioBox();
		~RadioBox();
		
		// Will select
		void AddChoice(const std::string& Choice);
		std::string GetChoice();

		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
		virtual Size GetSize();
		virtual void ForceResize(const Size& Size);
		virtual void OnResize(const Size& Size);
		virtual bool HandleInput(Input::Key Key, Input::IInputSystem& System);
		virtual bool CanFocus();
	};
}

#endif