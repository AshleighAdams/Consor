#include "BorderContainer.hpp"

using namespace Consor;
using namespace std;

CBorderContainer::CBorderContainer(CControl& Client, size_t Size)
{
	m_pClient = &Client;
	m_SetBorder(Size, Size, Size, Size);
}

CBorderContainer::CBorderContainer(CControl& Client, size_t Left, size_t Right, size_t Top, size_t Bottom)
{
	m_pClient = &Client;
	m_SetBorder(Left, Right, Top, Bottom);
}

void CBorderContainer::m_SetBorder(size_t Left, size_t Right, size_t Top, size_t Bottom)
{
	m_BorderLeft = Left;
	m_BorderRight = Right;
	m_BorderTop = Top;
	m_BorderBottom = Bottom;
}

CSize CBorderContainer::Size()
{
	CSize bordersize = CSize(m_BorderLeft + m_BorderRight, m_BorderTop + m_BorderBottom);
	return m_pClient->Size() + bordersize;
}

void CBorderContainer::OnResize(const CSize& Size)
{
}

void CBorderContainer::ForceResize(const CSize& Size)
{
	CSize bordersize = CSize(m_BorderLeft + m_BorderRight, m_BorderTop + m_BorderBottom);
	m_pClient->ForceResize(Size - bordersize); // subtract the border
}

void CBorderContainer::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	Renderer.PushRenderBounds(CVector(m_BorderLeft, m_BorderTop), m_pClient->Size());
	{
		m_pClient->Draw(Renderer, HasFocus, Skin);
	}
	Renderer.PopRenderBounds();
}

bool CBorderContainer::HandleInput(int Key)
{
	return m_pClient->HandleInput(Key);
}

bool CBorderContainer::CanFocus()
{
	return m_pClient->CanFocus();
}