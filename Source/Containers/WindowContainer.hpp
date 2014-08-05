#ifndef CONSOR_WINDOWCONTAINER_H
#define CONSOR_WINDOWCONTAINER_H

#include "../Control.hpp"
#include <string>

namespace Consor
{
	/// A window-like container.
	class WindowContainer : public Control
	{
	protected:
		Control& _Client;
		std::string _Title;
	public:
		WindowContainer(Control& Client, const std::string& Title);
		virtual Size GetSize();
		virtual void OnResize(const Size& Size);
		virtual void ForceResize(const Size& Size);
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
		virtual bool HandleInput(Input::Key Key, Input::IInputSystem& System);
		virtual bool CanFocus();
	};
};

#endif
