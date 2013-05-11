#include "TextBox.hpp"
#include "Util/StringUtils.hpp"
#include "Util/Time.hpp"
using namespace Consor;

CTextBox::CTextBox()
{
	m_Size = CSize();
	SetText("");
}

void CTextBox::SetText(const std::string& Text)
{
	m_Text = Text;
	m_CursorPosition = m_Text.length();
}

std::string CTextBox::GetText()
{
	return m_Text;
}

void CTextBox::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	CSize size = Size();

	Renderer.DrawBox(CVector(), size, Skin.TextBoxBackground());

	CColour fg =  Skin.TextBoxForeground();

	if(HasFocus)
		fg =  Skin.TextBoxForegroundFocused();

	if(size.Height == 1)
	{
		Renderer.DrawString("[", CVector(), fg, CColour::None());
		Renderer.DrawString("]", CVector(size.Width - 1, 0), fg, CColour::None());
	}
	else
		Renderer.DrawRect(CVector(), size, fg, Skin.TextBoxBackground());

	CVector pos = CVector(1, (int)(size.Height / 2));
	CSize textarea = CSize(size.Width - 2, 1);

	Renderer.PushRenderBounds(pos, textarea);
		pos = CVector();
		std::string text = GetText();
		int cursor_pos = (int)m_CursorPosition;

		bool dotdotdot_ = false;
		bool _dotdotdot = false;

		if(m_Text.length() > textarea.Width)
			_dotdotdot = true;

		if(cursor_pos >= textarea.Width - 3 && HasFocus)
		{
			pos.X -= cursor_pos - (textarea.Width - 1);
			
			dotdotdot_ = true;
			_dotdotdot = false;

			if(m_CursorPosition < m_Text.length() - 1)
			{
				_dotdotdot = true;
				pos.X -= 3;
			}

			cursor_pos -= pos.X;
		}

		Renderer.PushRenderBounds(pos, textarea);
			Renderer.DrawString(text, CVector(), Skin.TextBoxForeground(), CColour::None()); // this doesn't use focus col
			if(HasFocus && fmod(Util::GetTime(), 1.0) > 0.5)
			{
				std::unique_ptr<Console::ICharInformation> info = Renderer.GetCharInformation(pos + CVector(cursor_pos, 0));
				info->SetChar('_');
				info->SetForegroundColour(fg);
			}
		Renderer.PopRenderBounds();

		if(dotdotdot_)
			Renderer.DrawString("...", CVector(0, 0), Skin.TextBoxForeground(), CColour::None());
		

		if(_dotdotdot)
			Renderer.DrawString("...", CVector(textarea.Width - 3, 0), Skin.TextBoxForeground(), CColour::None());

	Renderer.PopRenderBounds();
}

bool CTextBox::m_IsTypedChar(Input::Key Key, char& letter)
{
	if(Key >= 32 && Key <= 126)
	{
		letter = (char)Key;
		return true;
	}

	return false;
}

bool CTextBox::HandleInput(Input::Key Key)
{
	if(Key == Input::Key::Left)
	{
		if(m_CursorPosition == 0)
			return false;

		m_CursorPosition--;
		return true;
	}
	else if(Key == Input::Key::Right)
	{
		if(m_CursorPosition == m_Text.length())
			return false;

		m_CursorPosition++;
		return true;
	}
	else if(Key == Input::Key::Delete)
	{
		if(m_CursorPosition == m_Text.length())
			return true;
		m_Text.erase(m_CursorPosition, 1);
		return true;
	}
	else if(Key == Input::Key::Backspace)
	{
		if(m_CursorPosition == 0)
			return true;
		m_Text.erase(m_CursorPosition - 1, 1);
		m_CursorPosition--;
		return true;
	}

	char letter;
	if(m_IsTypedChar(Key, letter))
	{
		m_Text.insert(m_CursorPosition, 1, letter);
		m_CursorPosition++;
		return true;
	}

	return false;
}


bool CTextBox::CanFocus()
{
	return true;
}