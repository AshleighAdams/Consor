#include "WindowContainer.hpp"

using namespace Consor;
using namespace std;

WindowContainer::WindowContainer(CControl& Client, const string& Title)
{
	_pClient = &Client;
	_Title = Title;
}

WindowContainer::WindowContainer()
{
	_pClient = nullptr;
	_Title = "None";
}

Size WindowContainer::GetSize()
{
	return _pClient->GetSize() + Size(2, 2); // an aditional 2 for the border
}

void WindowContainer::OnResize(const Size& size)
{
}

void WindowContainer::ForceResize(const Size& size)
{
	_pClient->ForceResize(size - Size(2, 2)); // subtract the border
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

	Renderer.PushRenderBounds(Vector(1, 1), _pClient->GetSize());
	{
		_pClient->Draw(Renderer, HasFocus, Skin);
	}
	Renderer.PopRenderBounds();
}

bool WindowContainer::HandleInput(Input::Key Key, Input::IInputSystem& System)
{
	return _pClient->HandleInput(Key, System);
}

bool WindowContainer::CanFocus()
{
	return true;
}