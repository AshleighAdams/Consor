#ifndef CONSOR_WINDOWSYSTEM_H
#define CONSOR_WINDOWSYSTEM_H

#include "ConsoleRenderer.hpp"
#include "Control.hpp"
#include <mutex>
#include <memory>
#include <thread>
#include <functional>

namespace Consor
{
	struct windowinfo_t
	{
		Control* pControl;
		Vector position;
	};

	
	namespace WindowSystem
	{
		namespace _priv_WindowSystem
		{
			extern void _SetSkin(std::shared_ptr<ISkin> skin);
		};
		
		/// Setup the window system with the passed console renderer and input system.
		extern bool Setup(Console::IConsoleRenderer* Renderer, Input::IInputSystem* input);

		/// \return A reference to the renderer.
		extern Console::IConsoleRenderer& Renderer();
		/// Tell the renderer to draw.
		extern void Draw();
		/// For thready safty when interfacing from other threads.
		extern void Lock();
		/// For thready safty when interfacing from other threads.
		extern void Unlock();
		extern void HandleInput(Input::Key key, Input::IInputSystem& is);
		/// Register and place a new control at the top of the stack.
		/// \param control The control to be registered.
		/// \param pos The position to place it.  `-1` means center, a value between `0` and `1` is a percentage, and values `>= 1` are absolute.
		extern void RegisterWindow(Control& control, Vector pos);
		/// Remove a control from the stack.
		extern void UnregisterWindow(Control& control);

		// automatically unregistered when pControl is unregistered (if not nullptr)
		/// Registers a hotkey, and calls `callback` when it is pushed.
		/// \param pControl The control the hotkey belongs to; should idealy be a window.  If this is not `nullptr`, then when the control is unregistered via `UnregisterWindow()`, the hotkey is removed.
		/// \param Key The key needed to activate the hotkey.
		/// \param Control Whether the control modifier needs to be pressed.
		/// \param Shift Whether the shift modifier needs to be pressed.
		/// \param callback The function to call upon the hotkey being activated.
		void RegisterHotKey(Control* pControl, Input::Key Key, bool Control, bool Shift, std::function<void()> callback);

		extern bool Running();
		extern void Close();
		extern std::string RendererName();
		extern std::string RendererVersionString();

		Colour RequestColour(const Colour& col, bool make = false);

		/// Set the skin for the window system to use.
		/// \tparam Skin The skin to set.
		template <class Skin>
		static std::shared_ptr<ISkin> SetSkin()
		{
			using namespace _priv_WindowSystem;
			Lock();
			std::shared_ptr<ISkin> skin = std::make_shared<Skin>(Renderer());
			//memleak fix: std::shared_ptr<ISkin>(new Skin(Renderer()));
			_SetSkin(skin);
			Unlock();
			return skin;
		}

		/// Set the skin for the window system to use.
		/// \param Skin The skin to set.
		static std::shared_ptr<ISkin> SetSkin(std::shared_ptr<ISkin> Skin)
		{
			using namespace _priv_WindowSystem;
			Lock();
			_SetSkin(Skin);
			Unlock();
			return Skin;
		}
	};
};

#endif
