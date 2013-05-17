#include "ConsoleRenderer.hpp"
#include <assert.h>
#include <locale>
#include <codecvt>

using namespace std;

using namespace Consor;
using namespace Consor::Console;

string IConsoleRenderer::RendererName()
{
	return "AbstractConsoleRenderer";
}

void IConsoleRenderer::Clear(const Colour& col)
{
	DrawBox(Vector(), GetSize(), col);
}

void IConsoleRenderer::DrawBox(const Vector& pos, const Size& size, const Colour& col)
{
	char box = ' ';

	unique_ptr<ICharInformation> info = GetCharInformation(Vector());

	for(int x = pos.X; x < pos.X + size.Width; x++)
		for(int y = pos.Y; y < pos.Y + size.Height; y++)
		{
			info->SetPosition(Vector(x, y));
			info->SetChar(box);
			info->SetBackgroundColour(col);
		}
}
/*
bool IConsoleRenderer::SupportsUnicode()
{
	return true;
}
*/

void IConsoleRenderer::DrawString(const std::string& str, const Vector& Pos, const Colour& fgcol, const Colour& bgcol)
{
	Vector pos = Pos;
	u32string unistr = wstring_convert<codecvt_utf8_utf16<char32_t>, char32_t>().from_bytes(str);

	bool unicode = SupportsUnicode();

	unique_ptr<ICharInformation> info = GetCharInformation(pos);

	for(const char32_t& letter : unistr)
	{
		info->SetPosition(pos);

		if(letter == '\n')
		{
			pos.X = Pos.X;
			pos.Y++;
			continue;
		}
		
		if(unicode)
			info->SetUnicodeChar(letter);
		else
			info->SetChar(letter);

		info->SetBackgroundColour( Colour::Blend(bgcol, info->GetBackgroundColour()) );
		info->SetForegroundColour( Colour::Blend(fgcol, info->GetForegroundColour()) );
		
		pos.X++;
	}
}

void IConsoleRenderer::PushRenderBounds(const Vector& from_a, const Size& size_a)
{
	renderbound_t rb;

	Vector from = Vector((int)from_a.X, (int)from_a.Y);
	Size size = Size((int)size_a.Width, (int)size_a.Height);
	
	//rb.Pos = from + _CurrentRenderBound.Pos; // this is added as the new renderbounds will apear to be the full console size
	rb.Pos = from + _CurrentOffset; // this should be correc,t but not fully tested...
	rb.Size = size;

	Vector offset = rb.Pos;
	
	if(_Bounds.size() != 0)
	{
		if(rb.Pos.X < _CurrentRenderBound.Pos.X)
			rb.Pos.X = _CurrentRenderBound.Pos.X;
		if(rb.Pos.X > _CurrentRenderBound.Pos.X + _CurrentRenderBound.Size.Width)
			rb.Size.Width = 0; // too far over to the right...
		if(rb.Pos.X + rb.Size.Width > _CurrentRenderBound.Pos.X + _CurrentRenderBound.Size.Width)
			rb.Size.Width = (_CurrentRenderBound.Pos.X + _CurrentRenderBound.Size.Width) - rb.Pos.X;

		if(rb.Pos.Y < _CurrentRenderBound.Pos.Y)
			rb.Pos.Y = _CurrentRenderBound.Pos.Y;
		if(rb.Pos.Y > _CurrentRenderBound.Pos.Y + _CurrentRenderBound.Size.Height)
			rb.Size.Height = 0; // too far over to the right...
		if(rb.Pos.Y + rb.Size.Height > _CurrentRenderBound.Pos.Y + _CurrentRenderBound.Size.Height)
			rb.Size.Height = (_CurrentRenderBound.Pos.Y + _CurrentRenderBound.Size.Height) - rb.Pos.Y;
	}

	_Bounds.push_back(rb);
	_Offsets.push_back(offset);

	_CurrentRenderBound = rb;
	_CurrentOffset = offset;
}

void IConsoleRenderer::PopRenderBounds()
{
	_Bounds.pop_back();
	_Offsets.pop_back();

	assert(_Bounds.size() > 0);

	_CurrentRenderBound = _Bounds.back(); // last element
	_CurrentOffset = _Offsets.back();
}

Size IConsoleRenderer::RenderSize()
{
	return Size(_CurrentRenderBound.Size.Width, _CurrentRenderBound.Size.Height);
}


/*
 MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY
 MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY
 MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY
 MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY
 MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY MESSY
*/



class RectangleChar
{
	int _State;
	bool _Unicode;
public:
	const static int LEFT	= (1 << 0);
	const static int RIGHT	= (1 << 1);
	const static int UP		= (1 << 2);
	const static int DOWN	= (1 << 3);
	
	RectangleChar(bool unicode, const bool left, const bool right, const bool up, const bool down)
	{
		_State = 0;
		_Unicode = unicode;

		if(left)
			_State |= LEFT;
		if(right)
			_State |= RIGHT;
		if(up)
			_State |= UP;
		if(down)
			_State |= DOWN;
	}
		
	RectangleChar(bool unicode, char32_t type)
	{
		_Unicode = unicode;

		#define CHAR_STATE(ltr, val) \
		case ltr: \
			_State = (val); \
			break
		if(!_Unicode)
		{
			switch(type)
			{
				CHAR_STATE(1, LEFT);
				CHAR_STATE(2, RIGHT);
				CHAR_STATE(3, UP);
				CHAR_STATE(4, DOWN);
			
				CHAR_STATE(179, UP | DOWN);
				CHAR_STATE(180, UP | DOWN | LEFT);
				CHAR_STATE(191, DOWN | LEFT);
				CHAR_STATE(192, UP | RIGHT);
				CHAR_STATE(193, LEFT | UP | RIGHT);
				CHAR_STATE(194, LEFT | DOWN | RIGHT);
				CHAR_STATE(195, UP | DOWN | RIGHT);
				CHAR_STATE(196, LEFT | RIGHT);
				CHAR_STATE(197, LEFT | RIGHT | UP | DOWN);
				CHAR_STATE(217, UP | LEFT);
				CHAR_STATE(218, RIGHT | DOWN);

			default:
				_State = 0;
			}
		}
		else
		{
			switch(type)
			{
				CHAR_STATE(0x2574, LEFT);
				CHAR_STATE(0x2576, RIGHT);
				CHAR_STATE(0x2575, UP);
				CHAR_STATE(0x2577, DOWN);
			
				CHAR_STATE(0x2502, UP | DOWN);
				CHAR_STATE(0x2524, UP | DOWN | LEFT);
				CHAR_STATE(0x2510, DOWN | LEFT);
				CHAR_STATE(0x2514, UP | RIGHT);
				CHAR_STATE(0x2534, LEFT | UP | RIGHT);
				CHAR_STATE(0x252C, LEFT | DOWN | RIGHT);
				CHAR_STATE(0x251C, UP | DOWN | RIGHT);
				CHAR_STATE(0x2500, LEFT | RIGHT);
				CHAR_STATE(0x253C, LEFT | RIGHT | UP | DOWN);
				CHAR_STATE(0x2518, UP | LEFT);
				CHAR_STATE(0x250C, RIGHT | DOWN);

			default:
				_State = 0;
			}
		}

		#undef CHAR_STATE
	}

	char32_t GetChar()
	{
		 // same as the one up there ^, but the only difference is it translated the other way
		#define CHAR_STATE(ltr, val) \
		case (val): \
			return (char32_t)ltr;

		if(!_Unicode)
		{
			switch(_State)
			{
				CHAR_STATE(1, LEFT);
				CHAR_STATE(2, RIGHT);
				CHAR_STATE(3, UP);
				CHAR_STATE(4, DOWN);

				CHAR_STATE(179, UP | DOWN);
				CHAR_STATE(180, UP | DOWN | LEFT);
				CHAR_STATE(191, DOWN | LEFT);
				CHAR_STATE(192, UP | RIGHT);
				CHAR_STATE(193, LEFT | UP | RIGHT);
				CHAR_STATE(194, LEFT | DOWN | RIGHT);
				CHAR_STATE(195, UP | DOWN | RIGHT);
				CHAR_STATE(196, LEFT | RIGHT);
				CHAR_STATE(197, LEFT | RIGHT | UP | DOWN);
				CHAR_STATE(217, UP | LEFT);
				CHAR_STATE(218, RIGHT | DOWN);

			default:
				return '?';
			}
		}
		else
		{
			switch(_State)
			{
				CHAR_STATE(0x2574, LEFT);
				CHAR_STATE(0x2576, RIGHT);
				CHAR_STATE(0x2575, UP);
				CHAR_STATE(0x2577, DOWN);
			
				CHAR_STATE(0x2502, UP | DOWN);
				CHAR_STATE(0x2524, UP | DOWN | LEFT);
				CHAR_STATE(0x2510, DOWN | LEFT);
				CHAR_STATE(0x2514, UP | RIGHT);
				CHAR_STATE(0x2534, LEFT | UP | RIGHT);
				CHAR_STATE(0x252C, LEFT | DOWN | RIGHT);
				CHAR_STATE(0x251C, UP | DOWN | RIGHT);
				CHAR_STATE(0x2500, LEFT | RIGHT);
				CHAR_STATE(0x253C, LEFT | RIGHT | UP | DOWN);
				CHAR_STATE(0x2518, UP | LEFT);
				CHAR_STATE(0x250C, RIGHT | DOWN);

			default:
				_State = 0;
			}
		}


		#undef CHAR_STATE
	}

	RectangleChar operator+(const RectangleChar& other)
	{
		RectangleChar ret(_Unicode, 0);
		ret._State = this->_State | other._State;

		return ret;
	}

	RectangleChar operator-(const RectangleChar& other)
	{
		RectangleChar ret(_Unicode, 0);
		ret._State = this->_State & ~other._State;

		return ret;
	}
};

void IConsoleRenderer::DrawRect(const Vector& pos, const Size& size, const Colour& fgcol, const Colour& bgcol)
{
	bool unicode = SupportsUnicode();
	RectangleChar Horizontal(unicode, true, true, false, false);
	RectangleChar Vertical(unicode, false, false, true, true);

	RectangleChar Left(unicode, true, false, false, false);
	RectangleChar Right(unicode, false, true, false, false);

	RectangleChar Top(unicode, false, false, true, false);
	RectangleChar Bottom(unicode, false, false, false, true);
	
	unique_ptr<ICharInformation> info_top = GetCharInformation(Vector());
	unique_ptr<ICharInformation> info_bot = GetCharInformation(Vector());
	unique_ptr<ICharInformation> info_l = GetCharInformation(Vector());
	unique_ptr<ICharInformation> info_r = GetCharInformation(Vector());

	if(size.Width == 1)
		;
	else if(size.Height == 1)
		for(int x = pos.X; x < pos.X + size.Width; x++)
		{
			info_top->SetPosition(Vector(x, pos.Y));
			
			info_top->SetUnicodeChar((RectangleChar(unicode, info_top->GetUnicodeChar()) + Horizontal).GetChar());
			info_top->SetForegroundColour(Colour::Blend(fgcol, info_top->GetForegroundColour()));
			info_top->SetBackgroundColour(Colour::Blend(bgcol, info_top->GetBackgroundColour()));
		}
	else
	for(int x = pos.X; x < pos.X + size.Width; x++)
	{
		info_top->SetPosition(Vector(x, pos.Y));
		info_bot->SetPosition(Vector(x, pos.Y + size.Height - 1));

		RectangleChar cur = Horizontal;

		if(x == pos.X)
			cur = cur - Left;
		else if(x == pos.X + size.Width - 1)
			cur = cur - Right;

		info_top->SetUnicodeChar((RectangleChar(unicode, info_top->GetUnicodeChar()) + cur).GetChar());
		info_top->SetForegroundColour(fgcol);
		info_top->SetBackgroundColour(bgcol);

		info_bot->SetUnicodeChar((RectangleChar(unicode, info_bot->GetUnicodeChar()) + cur).GetChar());
		info_bot->SetForegroundColour(fgcol);
		info_bot->SetBackgroundColour(bgcol);
	}
	
	if(size.Height == 1)
		;
	else if(size.Width == 1)
		for(int y = pos.Y; y < pos.Y + size.Height; y++)
		{
			info_l->SetPosition(Vector(pos.X, y));

			info_l->SetUnicodeChar( (RectangleChar(unicode, info_l->GetUnicodeChar()) + Vertical).GetChar() );
			info_l->SetForegroundColour(fgcol);
			info_l->SetBackgroundColour(bgcol);
		}
	else
	for(int y = pos.Y; y < pos.Y + size.Height; y++)
	{
		info_l->SetPosition(Vector(pos.X, y));
		info_r->SetPosition(Vector(pos.X + size.Width - 1, y));

		RectangleChar cur = Vertical;

		if(y == pos.Y)
			cur = cur - Top;
		else if(y == pos.Y + size.Height - 1)
			cur = cur - Bottom;

		info_l->SetUnicodeChar( (RectangleChar(unicode, info_l->GetUnicodeChar()) + cur).GetChar() );
		info_l->SetForegroundColour(fgcol);
		info_l->SetBackgroundColour(bgcol);

		info_r->SetUnicodeChar( (RectangleChar(unicode, info_r->GetUnicodeChar()) + cur).GetChar() );
		info_r->SetForegroundColour(fgcol);
		info_r->SetBackgroundColour(bgcol);
	}
}

bool IConsoleRenderer::InRenderBounds(const Vector& pos, Vector* dir)
{
	Size size = this->GetSize();
	bool ret = true;

	if(dir)
		*dir = Vector();

	int x = (int)pos.X, y = (int)pos.Y;
	x += (int)_CurrentOffset.X;
	y += (int)_CurrentOffset.Y;

	if(x < (int)(_CurrentRenderBound.Pos.X))
	{
		if(!dir)
			return false;
		dir->X = -1;
		ret = false;
	}
	if(y < (int)(_CurrentRenderBound.Pos.Y))
	{
		if(!dir)
			return false;
		dir->Y = -1;

		if(dir->X != 0) // both directioons have been found
			return false;

		ret = false;
	}

	if(x > (int)(_CurrentRenderBound.Pos.X + _CurrentRenderBound.Size.Width - 1))
	{
		if(!dir)
			return false;
		dir->X = 1;
		if(dir->Y != 0)
			return false;
		ret = false;
	}
	if(y > (int)(_CurrentRenderBound.Pos.Y + _CurrentRenderBound.Size.Height - 1))
	{
		if(!dir)
			return false;
		dir->Y = 1;
		if(dir->X != 0) // both set, ret false
			return false;
		ret = false;
	}

	if(x < 0 || (int)(x >= size.Width))
	{
		if(!ret)
			return false;
		dir->X = x < 0 ? -1 : 1;
		if(dir->Y != 0) // again, both are figgured wrong, return
			return false;
		ret = false;
	}
	if(y < 0 || (int)(y >= size.Height))
	{
		if(!ret)
			return false;
		dir->Y = y < 0 ? -1 : 1;
		ret = false;
	}

	return ret;
}

bool IConsoleRenderer::InRenderBounds(const Vector& pos, const Size& size)
{
	Vector dir1, dir2;

	Vector test1, test2;
	test1 = pos;
	test2 = pos + Vector(size.Width, size.Height);

	bool inside1 = this->InRenderBounds(test1, &dir1);
	bool inside2 = this->InRenderBounds(test2, &dir2);

	if(dir1.Y == dir2.Y && dir1.Y != 0) // completley out of bounds in Y
		return false;
	
	if(dir1.X == dir2.X && dir1.X != 0) // completley out of bounds in X
		return false;

	// there's something in bounds
	return true;
}