#include "HorizontalScrollbar.hpp"
#include "Util/Math.hpp"

using namespace Consor;

CHorizontalScrollbar::CHorizontalScrollbar(const CSize& Size)
{
	m_Value = 0;
	m_Size = Size;
	m_ChangeSize = -1;
}

CHorizontalScrollbar::CHorizontalScrollbar()
{
	m_Value = 0;
	m_ChangeSize = -1;
	m_Size = CSize();
}

void CHorizontalScrollbar::SetPercent(double Percent)
{
	m_Value = Percent;
}

double CHorizontalScrollbar::GetPercent()
{
	return m_Value;
}

void CHorizontalScrollbar::SetChangeSize(double ChangeSize)
{
	m_ChangeSize = ChangeSize;
}

double CHorizontalScrollbar::GetBarSize()
{
	if(m_ChangeSize <= 0)
		return 1;
	return m_ChangeSize * Size().Width;
}

void CHorizontalScrollbar::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	CColour fgcol = Skin.ScrollForeground();
	CColour bgcol = Skin.ScrollBackground();

	if(HasFocus)
		fgcol = Skin.ScrollForegroundFocused();

	CSize selfsize = this->Size();

	Renderer.DrawBox(CVector(), selfsize, bgcol);
	
	double barsize =  Util::Round(GetBarSize(), 0.5);
	if(barsize < 0.5)
		barsize = 0.5;

	CVector pos = CVector((selfsize.Width - barsize) * GetPercent(), 0);
	CSize size = CSize(barsize, selfsize.Height);

	//pos.Y = (int)pos.Y;

	char32_t lefthalf = 0x258C; //221;
	char32_t righthalf = 0x2590; //'▐';//222;
	char32_t full = 0x2588;

	if(!Renderer.SupportsUnicode())
	{
		lefthalf = 221;
		righthalf = 222;
		full = 219;
	}

	for(double x = pos.X; x < pos.X + size.Width; x += 0.5)
	{
		std::unique_ptr<Console::ICharInformation> info = Renderer.GetCharInformation(CVector(x, 0));
		char32_t cur = info->GetUnicodeChar();

		

		if(cur == lefthalf) // if top half already,
		{
			if(fmod(x, 1.0) >= 0.5) // and we venture into lower, set to full
				cur = full;
		}
		else if(cur == righthalf) // if bottom half already,
		{
			if(fmod(x, 1.0) <= 0.5) // and we venture into upper, set to full
				cur = full;
		}
		else // otherwise
		{
			if(fmod(x, 1.0) < 0.5) // if we're in the top half, set it
				cur = lefthalf;
			else // must be in the bottom, set that
				cur = righthalf;
		}

		info->SetUnicodeChar(cur);
		info->SetForegroundColour(fgcol);
	}

	//Renderer.DrawBox(pos, size, fgcol);

	return;

	if(fmod(pos.X, 1.0) > 0.5)
	{
		pos.X = (int)pos.X;

		std::unique_ptr<Console::ICharInformation> infoleft = Renderer.GetCharInformation(pos);
		std::unique_ptr<Console::ICharInformation> inforight = Renderer.GetCharInformation(pos + CVector(1, 0));

		infoleft->SetChar((char)221);
		inforight->SetChar((char)222);

		infoleft->SetForegroundColour(fgcol);
		inforight->SetForegroundColour(fgcol);
	}
	else
	{
		pos.X = (int)pos.X;
		std::unique_ptr<Console::ICharInformation> info = Renderer.GetCharInformation(pos);

		info->SetChar((char)219);
		info->SetForegroundColour(fgcol);
	}

	//Renderer.DrawBox(pos, size, fgcol);
}
