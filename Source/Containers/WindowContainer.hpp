#ifndef CONSOR_WINDOWCONTAINER_H
#define CONSOR_WINDOWCONTAINER_H

#include "../Control.hpp"
#include "../Units.hpp"

#include <string>
#include <mutex>

namespace Consor
{
	/// A window-like container.
	class WindowContainer : public Control
	{
	protected:
		Control& _Client;
		std::string _Title;
		/// Unlock this to allow Show() to return!
		std::mutex _RunningMutex;
		bool _Running;
	public:
		WindowContainer(Control& Client, const std::string& Title);
		~WindowContainer();
		/// Show the window (using WindowSystem), Position is the same as on RegisterWindow()
		void Show(const Vector& Position = Vector(-1, -1));
		void Close();
		
		virtual Size GetSize();
		virtual void OnResize(const Size& Size);
		virtual void ForceResize(const Size& Size);
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
		virtual bool HandleInput(Input::Key Key, Input::IInputSystem& System);
		virtual bool CanFocus();
	};
};

#endif
