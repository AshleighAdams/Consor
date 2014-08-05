#include "WindowSystem.hpp"
#include "Util/Time.hpp"
#include "Util/Debug.hpp"
#include "InputSystem.hpp"

#include <unordered_map>

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


struct _hotkey_info
{
	Input::Key key;
	bool ctrl;
	bool shift;
	function<void()> callback;
	void* ptr; // to prevent derefrencing
};

static std::list<_hotkey_info> _HotKeys;

void WindowSystem::RegisterHotKey(Control* pControl, Input::Key Key, bool Control, bool Shift, function<void()> callback)
{
	_Mutex.lock();

	_HotKeys.emplace_back();
	
	_hotkey_info& hotkey = _HotKeys.back();
	hotkey.callback = callback;
	hotkey.ctrl = Control;
	hotkey.shift = Shift;
	hotkey.key = Key;
	hotkey.ptr = pControl;
	
	_Mutex.unlock();
}

bool CheckHotkeys(Input::IInputSystem& System, Input::Key Key)
{
	bool ret = false;

	for(auto& hk : _HotKeys)
	{
		if(hk.key == Key && hk.ctrl == System.ControlDown() && hk.shift == System.ShiftDown())
		{
			ret = true;
			_Mutex.unlock();
			hk.callback();
			_Mutex.lock();
		}
	}

	return ret;
}

void RemoveHotkeys(void* ptr)
{
	_HotKeys.remove_if([&](const _hotkey_info& hk) -> bool
	{
		return hk.ptr == ptr;
	});
}

bool WindowSystem::Setup(Console::IConsoleRenderer* Renderer, Input::IInputSystem* input)
{
	static bool ran = false;

	if(ran) 
		return false;

	_pRenderer = Renderer;
	_pInput = input;

	if(!_pSkin) // allow a default to be set, don't force this on Setup
		_pSkin = std::make_shared<DefaultSkin>(*Renderer);
	//_pSkin = std::shared_ptr<ISkin>(new DefaultSkin(*Renderer));
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
			Util::Sleep(1.0/30.0); // run it roughly 30 times per second, about the max repeats/per second if you hold your key down
			
			while(_pInput->KeyWaiting())
			{
				Input::Key k = _pInput->GetKeyPress();
				HandleInput(k, *_pInput);
			}
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

	if(CheckHotkeys(is, key) || _Registered.begin() == _Registered.end())
	{
		_Mutex.unlock();
		return;
	}
	_Registered.back().pControl->HandleInput(key, is);

	_Mutex.unlock();
	Draw();
}

 void WindowSystem::RegisterWindow(Control& control, Vector pos)
 {
	windowinfo_t info;
	info.position = pos;
	info.pControl = &control;

	_Mutex.lock();
	_Registered.push_back(info);
	_Mutex.unlock();
	Draw();
 }

void WindowSystem::UnregisterWindow(Control& control)
{
	_Mutex.lock();
	RemoveHotkeys(&control);

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

Colour WindowSystem::RequestColour(const Colour& col, bool make)
{
	return _pRenderer->RequestColour(col, make);
}

std::string WindowSystem::RendererName()
{
	return Renderer().RendererName();
}

std::string WindowSystem::RendererVersionString()
{
	return Renderer().VersionString();
}
