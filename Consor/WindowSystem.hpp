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
			static std::mutex m_Mutex;
			static Console::IConsoleRenderer* m_pRenderer;
			static Input::IInputSystem* m_pInput;
			static std::shared_ptr<ISkin> m_pSkin;
			static std::thread m_InputThread;
			static std::thread m_DrawThread;
			static std::list<windowinfo_t> m_Registered;
			static bool m_Running;
			static bool m_Close;

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

		template <class Skin>
		static std::shared_ptr<ISkin> SetSkin()
		{
			using namespace _priv_WindowSystem;
			m_Mutex.lock();
			std::shared_ptr<ISkin> skin = std::shared_ptr<ISkin>(new Skin(Renderer()));
			m_SetSkin(skin);
			m_Mutex.unlock();
			return skin;
		}

		
	};
};

#endif