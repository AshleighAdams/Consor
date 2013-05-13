#include "WindowContainer.hpp"

using namespace Consor;
using namespace std;

CWindowContainer::CWindowContainer(CControl& Client, const string& Title)
{
	m_pClient = &Client;
	m_Title = Title;
}

CSize CWindowContainer::Size()
{
	return m_pClient->Size() + CSize(2, 2); // an aditional 2 for the border
}

void CWindowContainer::OnResize(const CSize& Size)
{
}

void CWindowContainer::ForceResize(const CSize& Size)
{
	m_pClient->ForceResize(Size - CSize(2, 2)); // subtract the border
}

void CWindowContainer::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	CVector pos;
	CSize size = Size();
	Renderer.DrawBox(pos, size, Skin.WindowBackground());
	Renderer.DrawRect(pos, size, Skin.WindowBorder(), CColour::None());

	CColour shine = Skin.WindowForegroundShine();
	if(shine != Skin.WindowBorder())
	{
		for(int x = 1; x < size.Width; x++)
		{
			unique_ptr<Console::ICharInformation> info = Renderer.GetCharInformation(pos + CVector(x, size.Height - 1));
			info->SetForegroundColour(shine);
		}
		for(int y = 0; y < size.Height; y++)
		{
			unique_ptr<Console::ICharInformation> info = Renderer.GetCharInformation(pos + CVector(size.Width - 1, y));
			info->SetForegroundColour(shine);
		}
	}

	CVector titlepos = CVector(size.Width / 2 - m_Title.length() / 2, 0);
	Renderer.DrawString(m_Title, titlepos, Skin.WindowForeground(), CColour::None());

	{
		unique_ptr<Console::ICharInformation> infol = Renderer.GetCharInformation(titlepos - CVector(1, 0));
		unique_ptr<Console::ICharInformation> infor = Renderer.GetCharInformation(titlepos + CVector(m_Title.length(), 0));

		infol->SetUnicodeChar(Skin.WindowTitleLeftChar()); // 180
		infor->SetUnicodeChar(Skin.WindowTitleRightChar()); // 195
	}

	// draw the child

	Renderer.PushRenderBounds(CVector(1, 1), m_pClient->Size());
	{
		m_pClient->Draw(Renderer, HasFocus, Skin);
	}
	Renderer.PopRenderBounds();
}

bool CWindowContainer::HandleInput(Input::Key Key, Input::IInputSystem& System)
{
	return m_pClient->HandleInput(Key, System);
}

bool CWindowContainer::CanFocus()
{
	return true;
}