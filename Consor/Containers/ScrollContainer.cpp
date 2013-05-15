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
	CSize hsize, vsize;
	if(m_Size.Width > 0)
		hsize = CSize(Size.Width - 1, 1); // The -1 because VScrollbar has priority
	else // set its size to nothing
		hsize = CSize(0, 0);
	
	if(m_Size.Height > 0)
		vsize = CSize(1, Size.Height - hsize.Height);
	else
		vsize = CSize(0, 0);

	m_HScrollbar.ForceResize(hsize);
	m_VScrollbar.ForceResize(vsize);
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
	CSize clientsize = m_pClient->Size();

	if(m_Size.Width > 0 && childsize.Width > m_Size.Width) // if the width isn't automatic
	{
		offset.X = (int)-(m_HScrollbar.GetPercent() * (childsize.Width - (selfsize.Width - vscrollsize.Width)));

		double incremant = 1.0 / (clientsize.Width - selfsize.Width);
		m_HScrollbar.SetChangeSize(incremant);

		Renderer.PushRenderBounds(CVector(0, selfsize.Height - hscrollsize.Height), hscrollsize);
			m_HScrollbar.Draw(Renderer, HasFocus, Skin);
		Renderer.PopRenderBounds();
	}
	else hscrollsize.Height = 0;

	if(m_Size.Height > 0 && childsize.Height > m_Size.Height) // if the hieht isn't automatic
	{
		offset.Y = (int)-(m_VScrollbar.GetPercent() * (childsize.Height - (selfsize.Height - hscrollsize.Height)));

		double incremant = 1.0 / (clientsize.Height - selfsize.Height);
		m_VScrollbar.SetChangeSize(incremant);

		Renderer.PushRenderBounds(CVector(selfsize.Width - vscrollsize.Width, 0), vscrollsize);
			m_VScrollbar.Draw(Renderer, HasFocus, Skin);
		Renderer.PopRenderBounds();
	}
	else vscrollsize.Width = 0;

	Renderer.PushRenderBounds(offset, selfsize - CSize(vscrollsize.Width, hscrollsize.Height));
		m_pClient->Draw(Renderer, HasFocus && m_pClient->CanFocus(), Skin);
	Renderer.PopRenderBounds();
}

bool CScrollContainer::ScrollDown(size_t count)
{
	CSize clientsize = m_pClient->Size();
	CSize selfsize = this->Size();

	if(clientsize.Height < selfsize.Height)
		return false;

	double perc = m_VScrollbar.GetPercent();
	double incremant = 1.0 / (clientsize.Height - selfsize.Height);

	perc += incremant * (double)count;

	if(perc > 1.0)
		perc = 1.0;

	if(perc == m_VScrollbar.GetPercent()) // allow shallower scrollcontainers to also scroll
		return false;

	m_VScrollbar.SetPercent(perc);
	return true;
}

bool CScrollContainer::ScrollUp(size_t count)
{
	CSize clientsize = m_pClient->Size();
	CSize selfsize = this->Size();

	if(clientsize.Height < selfsize.Height)
		return false;

	double perc = m_VScrollbar.GetPercent();
	double incremant = 1.0 / (clientsize.Height - selfsize.Height);

	perc -= incremant * (double)count;

	if(perc < 0)
		perc = 0;

	if(perc == m_VScrollbar.GetPercent()) // allow shallower scrollcontainers to also scroll
		return false;

	m_VScrollbar.SetPercent(perc);
	return true;
}

bool CScrollContainer::ScrollLeft(size_t count)
{
	CSize clientsize = m_pClient->Size();
	CSize selfsize = this->Size();

	if(clientsize.Width < selfsize.Width)
		return false;

	double perc = m_HScrollbar.GetPercent();
	double incremant = 1.0 / (clientsize.Width - selfsize.Width);

	perc -= incremant * (double)count;

	if(perc < 0)
		perc = 0;

	if(perc == m_HScrollbar.GetPercent()) // allow shallower scrollcontainers to also scroll
		return false;

	m_HScrollbar.SetPercent(perc);
	return true;
}

bool CScrollContainer::ScrollRight(size_t count)
{
	CSize clientsize = m_pClient->Size();
	CSize selfsize = this->Size();

	if(clientsize.Width < selfsize.Width)
		return false;
	double perc = m_HScrollbar.GetPercent();
	double incremant = 1.0 / (clientsize.Width - selfsize.Width);

	perc += incremant * (double)count;

	if(perc > 1.0)
		perc = 1.0;

	if(perc == m_HScrollbar.GetPercent()) // allow shallower scrollcontainers to also scroll
		return false;

	m_HScrollbar.SetPercent(perc);
	return true;
}

bool CScrollContainer::HandleInput(Input::Key Key, Input::IInputSystem& System)
{
	if(System.ControlDown())
	{
		char x = (char)Key;
		bool handeled = false;

		switch(Key)
		{
		case Input::Key::Left:
			handeled = ScrollLeft();
			break;
		case Input::Key::Right:
			handeled = ScrollRight();
			break;
		case Input::Key::Up:
			handeled = ScrollUp();
			break;
		case Input::Key::Down:
			handeled = ScrollDown();
			break;
		}

		if(handeled)
			return true;
	}

	if(m_pClient->HandleInput(Key, System))
		return true;

	if(m_Size.Height > 0)
	{
		const double context = 1.0 - 0.1; //10% context

		if(Key == Input::Key::PageDown || Key == Input::Key::Numpad3)
			return ScrollDown(m_Size.Height * context);
		else if(Key == Input::Key::PageUp || Key == Input::Key::Numpad9)
			return ScrollUp(m_Size.Height * context);
	}

	return false;
}

bool CScrollContainer::CanFocus()
{
	return true;
}