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
	Renderer.DrawBox(pos, Size(), Skin.WindowBackground());
	Renderer.DrawRect(pos, Size(), Skin.WindowBorder(), CColour::None());

	Renderer.DrawString(m_Title, CVector(size.Width / 2 - m_Title.length() / 2, 0), Skin.WindowForeground(), CColour::None());

	// draw the child

	Renderer.PushRenderBounds(CVector(1, 1), m_pClient->Size());
	{
		m_pClient->Draw(Renderer, HasFocus, Skin);
	}
	Renderer.PopRenderBounds();
}

bool CWindowContainer::HandleInput(Input::Key Key)
{
	return m_pClient->HandleInput(Key);
}

bool CWindowContainer::CanFocus()
{
	return true;
}