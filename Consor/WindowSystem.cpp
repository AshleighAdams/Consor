#include "WindowSystem.hpp"
#include "Util/Time.hpp"
#include "InputSystem.hpp"

using namespace Consor;
using namespace std;
using namespace WindowSystem::_priv_WindowSystem;

static std::mutex _Mutex;
static Console::IConsoleRenderer* _pRenderer;
static Input::IInputSystem* _pInput;
static std::shared_ptr<ISkin> _pSkin;
static std::thread _InputThread;
static std::thread _DrawThread;
static std::list<windowinfo_t> _Registered;
static bool _Running;
static bool _Close;

bool WindowSystem::Setup(Console::IConsoleRenderer* Renderer, Input::IInputSystem* input)
{
	static bool ran = false;

	if(ran) 
		return false;
	
	_pRenderer = Renderer;
	_pInput = input;
	_pSkin = std::shared_ptr<ISkin>(new CDefaultSkin(*Renderer));
	_Running = true;
	_Close = false;

	// create the threads
	_DrawThread = thread([&]()
	{
		while(!_Close)
		{
			WindowSystem::Draw();
			Util::Sleep(0.5);
		}
	});

	_InputThread = thread([&]()
	{
		while(!_Close)
		{
			Input::Key k = _pInput->GetKeyPress();
			HandleInput(k, *_pInput);
		}
	});

	return true;
}

void WindowSystem::_priv_WindowSystem::_SetSkin(std::shared_ptr<ISkin> skin)
{
	_pSkin = skin;
}

void WindowSystem::Draw()
{
	_Mutex.lock();

	Console::IConsoleRenderer& renderer = Renderer();

	renderer.Clear(_pSkin->Canvas());
	
	for(windowinfo_t& info : _Registered)
	{
		Consor::Size size = info.pControl->GetSize();
		Consor::Size rsize = renderer.RenderSize();
		Consor::Vector pos = info.position;

		if(info.position.X < 0)
			pos.X = rsize.Width / 2.0 - size.Width / 2;
		
		if(info.position.Y < 0)
			pos.Y = rsize.Height / 2.0 - size.Height / 2;

		renderer.PushRenderBounds(pos, size);
		info.pControl->Draw(renderer, &info == &_Registered.back(), *_pSkin);
		renderer.PopRenderBounds();
	}

	renderer.FlushToScreen();

	_Mutex.unlock();
}

void WindowSystem::HandleInput(Input::Key key, Input::IInputSystem& is)
{
	_Mutex.lock();

	if(_Registered.begin() == _Registered.end())
	{
		_Mutex.unlock();
		return;
	}
	_Registered.back().pControl->HandleInput(key, is);

	_Mutex.unlock();
	Draw();
}

 void WindowSystem::RegisterWindow(CControl& control, Vector pos)
 {
	windowinfo_t info;
	info.position = pos;
	info.pControl = &control;

	_Mutex.lock();
	_Registered.push_back(info);
	_Mutex.unlock();
	Draw();
 }

void WindowSystem::UnregisterWindow(CControl& control)
{
	_Mutex.lock();
	_Registered.remove_if([&](const windowinfo_t& info)
	{
		return info.pControl == &control;
	});
	_Mutex.unlock();
	Draw();
}

Console::IConsoleRenderer& WindowSystem::Renderer()
{
	return *_pRenderer;
}

void WindowSystem::Lock()
{
	_Mutex.lock();
}

void WindowSystem::Unlock()
{
	_Mutex.unlock();
}

void WindowSystem::Close()
{
	if(_Close)
		return;

	_Close = true;
	_DrawThread.join();
	_InputThread.join();
	_Running = false;
}

bool WindowSystem::Running()
{
	return _Running;
}

std::string WindowSystem::RendererName()
{
	return Renderer().RendererName();
}

std::string WindowSystem::RendererVersionString()
{
	return Renderer().VersionString();
}