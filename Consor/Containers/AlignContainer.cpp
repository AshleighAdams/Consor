#include "AlignContainer.hpp"

using namespace Consor;
using namespace std;

CAlignContainer::CAlignContainer(CControl& Client, const CSize& Size, CAlignContainer::Axis Axis, CAlignContainer::Align Align)
{
	m_pClient = &Client;
	m_Axis = Axis;
	m_Align = Align;
	m_Size = Size;
}

CSize CAlignContainer::Size()
{
	CSize ret;
	CSize clientsize = m_pClient->Size();

	if(m_Axis == CAlignContainer::Axis::Horizotal)
	{
		ret.Height = clientsize.Height;
		ret.Width = m_Size.Width;
	}
	else
	{
		ret.Height = m_Size.Height;
		ret.Width = clientsize.Width;
	}
	
	return ret;
}

CVector CAlignContainer::m_ClientPos()
{
	CSize size = Size();
	CSize clientsize = m_pClient->Size();

	CVector ret;

	if(m_Axis == CAlignContainer::Axis::Horizotal)
	{
		switch (m_Align)
		{
		case Consor::CAlignContainer::Right:
			ret.X = size.Width - clientsize.Width;
			break;
		case Consor::CAlignContainer::Center:
			ret.X = size.Width / 2.0 - clientsize.Width / 2.0;
			break;
		default:
			break;
		}
	}
	else
	{
		switch (m_Align)
		{
		case Consor::CAlignContainer::Bottom:
			ret.Y = size.Height - clientsize.Height;
			break;
		case Consor::CAlignContainer::Center:
			ret.Y = size.Height / 2.0 - clientsize.Height / 2.0;
			break;
		default:
			break;
		}
	}

	return ret;
}

void CAlignContainer::OnResize(const CSize& Size)
{
}

void CAlignContainer::ForceResize(const CSize& Size)
{
	m_Size = Size;
}

void CAlignContainer::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	Renderer.PushRenderBounds(m_ClientPos(), m_pClient->Size());
	m_pClient->Draw(Renderer, HasFocus, Skin);
	Renderer.PopRenderBounds();
}

bool CAlignContainer::HandleInput(Input::Key Key)
{
	return m_pClient->HandleInput(Key);
}

bool CAlignContainer::CanFocus()
{
	return m_pClient->CanFocus();
}