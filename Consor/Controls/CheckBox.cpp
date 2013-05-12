#include "CheckBox.hpp"
#include "Util/StringUtils.hpp"

using namespace Consor;

CCheckBox::CCheckBox()
{
	m_Label.SetText("Default Checkbox");
	m_Checked = false;
}

void CCheckBox::SetText(std::string Text)
{
	m_Label.SetText(Text);
}


void CCheckBox::SetChecked(bool Value)
{
	m_Checked = Value;
}

bool CCheckBox::Checked()
{
	return m_Checked;
}

void CCheckBox::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	CColour col = Skin.LabelForeground();

	if(HasFocus)
		col = Skin.LabelForegroundFocused();

	Renderer.DrawString("[ ]", CVector(), col, CColour::None());

	std::unique_ptr<Console::ICharInformation> info = Renderer.GetCharInformation(CVector(1, 0));
	
	if(Checked())
	{
		if(info->SupportsUnicode())
			info->SetUnicodeChar(0x25EF);
		else
			info->SetChar('*');
	}

	Renderer.PushRenderBounds(CVector(4, 0), m_Label.Size());
	m_Label.Draw(Renderer, false, Skin);
	Renderer.PopRenderBounds();
}

CSize CCheckBox::Size()
{
	return m_Label.Size() + CSize(4, 0);
}

void CCheckBox::OnResize(const CSize& Size)
{
	m_Label.ForceResize(Size - CSize(4, 0));
}

void CCheckBox::ForceResize(const CSize& Size)
{
	OnResize(Size);
}

bool CCheckBox::HandleInput(Input::Key Key)
{
	if(Key == Input::Key::Enter || Key == Input::Key::Space)
	{
		SetChecked(!Checked());
		return true;
	}

	return false;
}

bool CCheckBox::CanFocus()
{
	return true;
}