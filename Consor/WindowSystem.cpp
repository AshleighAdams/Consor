#include "WindowSystem.hpp"
#include "Util/Time.hpp"
#include "InputSystem.hpp"

using namespace Consor;
using namespace std;
using namespace WindowSystem::_priv_WindowSystem;

bool WindowSystem::Setup(Console::IConsoleRenderer* Renderer, Input::IInputSystem* input)
{
	static bool ran = false;

	if(ran) 
		return false;
	
	m_pRenderer = Renderer;
	m_pInput = input;
	m_pSkin = std::shared_ptr<ISkin>(new CDefaultSkin(*Renderer));
	m_Running = true;
	m_Close = false;

	// create the threads
	m_DrawThread = thread([&]()
	{
		while(!m_Close)
		{
			WindowSystem::Draw();
			Util::Sleep(0.5);
		}
	});

	m_InputThread = thread([&]()
	{
		while(!m_Close)
		{
			Input::Key k = m_pInput->GetKeyPress();
			HandleInput(k, *m_pInput);
			Draw();
		}
	});

	return true;
}

void WindowSystem::_priv_WindowSystem::m_SetSkin(std::shared_ptr<ISkin> skin)
{
	m_pSkin = skin;
}

void WindowSystem::Draw()
{
	m_Mutex.lock();

	Console::IConsoleRenderer& renderer = Renderer();

	renderer.Clear(m_pSkin->Canvas());
	
	for(windowinfo_t& info : m_Registered)
	{
		Consor::CSize size = info.pControl->Size();
		Consor::CSize rsize = renderer.RenderSize();
		Consor::CVector pos = info.position;

		if(info.position.X < 0)
			pos.X = rsize.Width / 2.0 - size.Width / 2;
		
		if(info.position.Y < 0)
			pos.Y = rsize.Height / 2.0 - size.Height / 2;

		renderer.PushRenderBounds(pos, size);
		info.pControl->Draw(renderer, &info == &m_Registered.back(), *m_pSkin);
		renderer.PopRenderBounds();
	}

	renderer.FlushToScreen();

	m_Mutex.unlock();
}

void WindowSystem::HandleInput(Input::Key key, Input::IInputSystem& is)
{
	m_Mutex.lock();

	if(m_Registered.begin() == m_Registered.end())
	{
		m_Mutex.unlock();
		return;
	}
	m_Registered.back().pControl->HandleInput(key, is);

	m_Mutex.unlock();
}

 void WindowSystem::RegisterWindow(CControl& control, CVector pos)
 {
	windowinfo_t info;
	info.position = pos;
	info.pControl = &control;

	m_Mutex.lock();
	m_Registered.push_back(info);
	m_Mutex.unlock();
 }

void WindowSystem::UnregisterWindow(CControl& control)
{
	m_Mutex.lock();
	m_Registered.remove_if([&](const windowinfo_t& info)
	{
		return info.pControl == &control;
	});
	m_Mutex.unlock();
}

Console::IConsoleRenderer& WindowSystem::Renderer()
{
	return *m_pRenderer;
}

void WindowSystem::Lock()
{
	m_Mutex.lock();
}

void WindowSystem::Unlock()
{
	m_Mutex.unlock();
}

void WindowSystem::Close()
{
	m_Close = true;
	m_DrawThread.join();
	m_InputThread.join();
	m_Running = false;
}

bool WindowSystem::Running()
{
	return m_Running;
}