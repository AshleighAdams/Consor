#include "WindowContainer.hpp"

#include "../Util/Time.hpp"
#include "../Util/Debug.hpp"
#include "../WindowSystem.hpp"

using namespace Consor;
using namespace std;

WindowContainer::WindowContainer(Control& Client, const string& Title) :
	_Client(Client),
	_Title(Title),
	_Running(false)
{
}

WindowContainer::~WindowContainer()
{
	struct DisposedButNotUnregistered {} _;
	if(_Running)
	{
		Util::Log("~WindowContainer() called, but is still registered!");
		this->Close();
	}
}

void WindowContainer::Show(const Vector& Position)
{
	if(_Running)
		return;
	_Running = true;
	
	WindowSystem::RegisterWindow(*this, Position);
	
	_RunningMutex.lock();
	_RunningMutex.lock(); // Lock the thread, eventually the input thread in the
	                      // WindowSystem will call `this->Close()` where the
	                      // mutex will be unlocked, and allowed to continue.
	
	_RunningMutex.unlock(); // for consistancy sake, remove the first lock.
	
	//while(_Running)
	//	Util::Sleep(1.0 / 10.0); // check 10 times per second	
}

void WindowContainer::Close()
{
	if(!_Running)
		return;
	
	WindowSystem::UnregisterWindow(*this);
	_Running = false;
	_RunningMutex.unlock();
}

Size WindowContainer::GetSize()
{
	return _Client.GetSize() + Size(2, 2); // an aditional 2 for the border
}

void WindowContainer::OnResize(const Size& size)
{
}

void WindowContainer::ForceResize(const Size& size)
{
	_Client.ForceResize(size - Size(2, 2)); // subtract the border
}

void WindowContainer::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	Vector pos;
	Size size = this->GetSize();
	Renderer.DrawBox(pos, size, Skin.WindowBackground());
	Renderer.DrawRect(pos, size, Skin.WindowBorder(), Colour::None());

	Colour shine = Skin.WindowForegroundShine();
	if(shine != Skin.WindowBorder())
	{
		for(int x = 1; x < size.Width; x++)
		{
			unique_ptr<Console::ICharInformation> info = Renderer.GetCharInformation(pos + Vector(x, size.Height - 1));
			info->SetForegroundColour(shine);
		}
		for(int y = 0; y < size.Height; y++)
		{
			unique_ptr<Console::ICharInformation> info = Renderer.GetCharInformation(pos + Vector(size.Width - 1, y));
			info->SetForegroundColour(shine);
		}
	}

	Vector titlepos = Vector(size.Width / 2 - _Title.length() / 2, 0);
	Renderer.DrawString(_Title, titlepos, Skin.WindowForeground(), Colour::None());

	{
		unique_ptr<Console::ICharInformation> infol = Renderer.GetCharInformation(titlepos - Vector(1, 0));
		unique_ptr<Console::ICharInformation> infor = Renderer.GetCharInformation(titlepos + Vector(_Title.length(), 0));

		infol->SetUnicodeChar(Skin.WindowTitleLeftChar()); // 180
		infor->SetUnicodeChar(Skin.WindowTitleRightChar()); // 195
	}

	// draw the child

	Renderer.PushRenderBounds(Vector(1, 1), _Client.GetSize());
	{
		_Client.Draw(Renderer, HasFocus, Skin);
	}
	Renderer.PopRenderBounds();
}

bool WindowContainer::HandleInput(Input::Key Key, Input::IInputSystem& System)
{
	return _Client.HandleInput(Key, System);
}

bool WindowContainer::CanFocus()
{
	return true;
}
