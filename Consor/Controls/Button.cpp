#include "Button.hpp"
#include "Util/StringUtils.hpp"

using namespace Consor;

CButton::CButton()
{
	m_Label.SetText("Default Button");
	m_HAlign = CAlignContainer(m_Label, m_Label.Size(), 
		CAlignContainer::Axis::Horizotal, CAlignContainer::Align::Center);
	m_VAlign = CAlignContainer(m_HAlign, m_Label.Size(), 
		CAlignContainer::Axis::Vertical, CAlignContainer::Align::Center);
}

void CButton::SetText(std::string Text)
{
	m_Label.SetText(Text);
	ForceResize(m_Label.Size() + CSize(2, 0));
	//OnResize(m_Size);
}

void CButton::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	CColour col = Skin.LabelForeground();

	if(HasFocus)
		col = Skin.LabelForegroundFocused();

	Renderer.PushRenderBounds(CVector(1, 0), m_VAlign.Size());
	m_VAlign.Draw(Renderer, HasFocus, Skin);
	Renderer.PopRenderBounds();

	if(Size().Height == 1)
	{
		Renderer.DrawString("<", CVector(0, m_Size.Height / 2.0), col, CColour::None());
		Renderer.DrawString(">", CVector(m_Size.Width - 1, m_Size.Height / 2.0), col, CColour::None());
	}
	else
	{
		Renderer.DrawRect(CVector(), Size(), col, CColour::None());
	}
}

void CButton::OnResize(const CSize& Size)
{
	m_Label.ForceResize(Size - CSize(2, 0));
	m_HAlign.ForceResize(Size - CSize(2, 0));
	m_VAlign.ForceResize(Size - CSize(2, 0));
}

void CButton::ForceResize(const CSize& Size)
{
	m_Size = Size;
	OnResize(m_Size);
}

bool CButton::HandleInput(Input::Key Key, Input::IInputSystem& System)
{
	if(Key == Input::Key::Enter || Key == Input::Key::Space)
	{
		Click();
		return true;
	}

	return false;
}

bool CButton::CanFocus()
{
	return true;
}