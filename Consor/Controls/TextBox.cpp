#include "TextBox.hpp"
#include "../Util/StringUtils.hpp"
#include "../Util/Time.hpp"
using namespace Consor;

TextBox::TextBox()
{
	_Size = Size(20, 1);
	this->SetText("");
	_InsertMode = false;
	_LastTyped = Util::GetTime();
}

void TextBox::SetText(const std::string& Text)
{
	_Text = Text;
	_CursorPosition = _Text.length();
}

std::string TextBox::GetText()
{
	return _Text;
}

void TextBox::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	Size size = this->GetSize();

	Renderer.DrawBox(Vector(), size, Skin.TextBoxBackground());

	Colour fg =  Skin.TextBoxForeground();

	if(HasFocus)
		fg =  Skin.TextBoxForegroundFocused();

	if(size.Height == 1)
	{
		Renderer.DrawString("[", Vector(), fg, Colour::None());
		Renderer.DrawString("]", Vector(size.Width - 1, 0), fg, Colour::None());
	}
	else
		Renderer.DrawRect(Vector(), size, fg, Skin.TextBoxBackground());

	Vector pos = Vector(1, (int)(size.Height / 2));
	Size textarea = Size(size.Width - 2, 1);

	Renderer.PushRenderBounds(pos, textarea);
		pos = Vector();
		std::string text = GetText();
		int cursor_pos = (int)_CursorPosition;

		bool dotdotdot_ = false;
		bool _dotdotdot = false;

		if(_Text.length() > textarea.Width)
			_dotdotdot = true;

		if(cursor_pos >= textarea.Width - 3 && HasFocus)
		{
			pos.X -= cursor_pos - (textarea.Width - 1);
			
			dotdotdot_ = true;
			_dotdotdot = false;

			if(_CursorPosition < _Text.length() - 1)
			{
				_dotdotdot = true;
				pos.X -= 3;
			}

			cursor_pos -= pos.X;
		}

		Renderer.PushRenderBounds(pos, textarea);
			Renderer.DrawString(text, Vector(), Skin.TextBoxForeground(), Colour::None()); // this doesn't use focus col
			if(HasFocus && fmod(Util::GetTime() - _LastTyped, 1.0) < 0.5)
			{
				std::unique_ptr<Console::ICharInformation> info = Renderer.GetCharInformation(pos + Vector(cursor_pos, 0));
				info->SetChar(_InsertMode ? (char)219 : '_');
				info->SetForegroundColour(fg);
			}
		Renderer.PopRenderBounds();

		if(dotdotdot_)
			Renderer.DrawString("...", Vector(0, 0), Skin.TextBoxForeground(), Colour::None());
		

		if(_dotdotdot)
			Renderer.DrawString("...", Vector(textarea.Width - 3, 0), Skin.TextBoxForeground(), Colour::None());

	Renderer.PopRenderBounds();
}

bool TextBox::_IsTypedChar(Input::Key Key, char& letter)
{
	if(Key >= 32 && Key <= 126)
	{
		letter = (char)Key;
		return true;
	}

	return false;
}

bool TextBox::HandleInput(Input::Key Key, Input::IInputSystem& System)
{
	if(Key == Input::Key::Left)
	{
		if(_CursorPosition == 0)
			return false;

		_CursorPosition--;
		_LastTyped = Util::GetTime();
		return true;
	}
	else if(Key == Input::Key::Right)
	{
		if(_CursorPosition == _Text.length())
			return false;

		_CursorPosition++;
		_LastTyped = Util::GetTime();
		return true;
	}
	else if(Key == Input::Key::Delete)
	{
		if(_CursorPosition == _Text.length())
			return true;
		_Text.erase(_CursorPosition, 1);
		_LastTyped = Util::GetTime();
		return true;
	}
	else if(Key == Input::Key::Backspace)
	{
		if(_CursorPosition == 0)
			return true;
		_Text.erase(_CursorPosition - 1, 1);
		_CursorPosition--;
		_LastTyped = Util::GetTime();
		return true;
	}
	else if(Key == Input::Key::Insert)
	{
		_InsertMode = !_InsertMode;
		_LastTyped = Util::GetTime();
		return true;
	}

	char letter;
	if(_IsTypedChar(Key, letter))
	{
		if(_InsertMode)
			_Text.erase(_CursorPosition, 1);
		_Text.insert(_CursorPosition, 1, letter);
		_CursorPosition++;
		_LastTyped = Util::GetTime();
		return true;
	}

	return false;
}


bool TextBox::CanFocus()
{
	return true;
}
