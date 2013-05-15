#ifndef CONSOR_WINDOWSYSTEM_H
#define CONSOR_WINDOWSYSTEM_H

#include "ConsoleRenderer.hpp"
#include "Control.hpp"
#include <mutex>
#include <memory>
#include <thread>

namespace Consor
{
	struct windowinfo_t
	{
		CControl* pControl;
		CVector position;
	};

	
	namespace WindowSystem
	{
		namespace _priv_WindowSystem
		{
			extern void m_SetSkin(std::shared_ptr<ISkin> skin);
		};

		extern bool Setup(Console::IConsoleRenderer* Renderer, Input::IInputSystem* input);

		extern Console::IConsoleRenderer& Renderer();
		extern void Draw();
		extern void Lock();
		extern void Unlock();
		extern void HandleInput(Input::Key key, Input::IInputSystem& is);
		extern void RegisterWindow(CControl& control, CVector pos);
		extern void UnregisterWindow(CControl& control);

		extern bool Running();
		extern void Close();
		extern std::string RendererName();
		extern std::string RendererVersionString();

		template <class Skin>
		static std::shared_ptr<ISkin> SetSkin()
		{
			using namespace _priv_WindowSystem;
			Lock();
			std::shared_ptr<ISkin> skin = std::shared_ptr<ISkin>(new Skin(Renderer()));
			m_SetSkin(skin);
			Unlock();
			return skin;
		}

		
	};
};

#endif