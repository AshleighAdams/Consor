#include "ScrollContainer.hpp"

using namespace Consor;
using namespace std;

CScrollContainer::CScrollContainer(CControl& Client, const CSize& Size)
{
	m_pClient = &Client;
	ForceResize(Size);
}

CSize CScrollContainer::Size()
{
	CSize clientsize = m_pClient->Size();
	CSize ret = m_Size;

	if(m_Size.Width < 0)
		ret.Width = clientsize.Width + m_VScrollbar.Size().Width;
	if(m_Size.Height < 0)
		ret.Height = clientsize.Height + m_HScrollbar.Size().Height;

	return ret;
}

void CScrollContainer::OnResize(const CSize& Size)
{
	if(m_Size.Width > 0)
		m_HScrollbar.ForceResize(CSize(Size.Width - 1, 1)); // The -1 because HScrollbar has priority
	else // set its size to nothing
		m_HScrollbar.ForceResize(CSize(0, 0));
	
	if(m_Size.Height > 0)
		m_VScrollbar.ForceResize(CSize(1, Size.Height));
	else
		m_VScrollbar.ForceResize(CSize(0, 0));
}

void CScrollContainer::ForceResize(const CSize& Size)
{
	m_Size = Size;
	OnResize(this->Size());
}

void CScrollContainer::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	CSize selfsize = Size();
	CSize childsize = m_pClient->Size();

	CVector offset;

	CSize vscrollsize = m_VScrollbar.Size();
	CSize hscrollsize = m_HScrollbar.Size();

	if(m_Size.Width > 0) // if the width isn't automatic
	{
		offset.X = (int)-(m_HScrollbar.GetPercent() * childsize.Width);

		Renderer.PushRenderBounds(CVector(0, selfsize.Height - hscrollsize.Height), hscrollsize);
			m_HScrollbar.Draw(Renderer, false, Skin);
		Renderer.PopRenderBounds();
	}
	if(m_Size.Height > 0) // if the hieht isn't automatic
	{
		offset.Y = (int)-(m_VScrollbar.GetPercent() * childsize.Height);

		Renderer.PushRenderBounds(CVector(selfsize.Width - vscrollsize.Width, 0), vscrollsize);
			m_VScrollbar.Draw(Renderer, false, Skin);
		Renderer.PopRenderBounds();
	}

	Renderer.PushRenderBounds(offset, selfsize - CSize(vscrollsize.Width, hscrollsize.Height));
		m_pClient->Draw(Renderer, HasFocus, Skin);
	Renderer.PopRenderBounds();
}

bool CScrollContainer::HandleInput(Input::Key Key)
{
	if(m_pClient->HandleInput(Key))
		return true;

	CSize clientsize = m_pClient->Size();

	if(m_Size.Height > 0)
	{
		if(Key == Input::Key::PageDown || Key == Input::Key::Numpad3)
		{
			double perc = m_VScrollbar.GetPercent();
			double incremant = 1.0 / clientsize.Height;

			perc += incremant;

			if(perc > 1.0)
				perc = 1.0;
			m_VScrollbar.SetPercent(perc);
		}
		else if(Key == Input::Key::PageUp || Key == Input::Key::Numpad9)
		{
			double perc = m_VScrollbar.GetPercent();
			double incremant = 1.0 / clientsize.Height;

			perc -= incremant;

			if(perc < 0)
				perc = 0;
			m_VScrollbar.SetPercent(perc);
		}
	}
}

bool CScrollContainer::CanFocus()
{
	return true;
}