#include "VerticalScrollbar.hpp"

#include "Util/Math.hpp"

#include <string>
#include <cmath>
using namespace Consor;

CVerticalScrollbar::CVerticalScrollbar(const CSize& Size)
{
	m_Value = 0;
	m_Size = Size;
	m_ChangeSize = -1;
}

CVerticalScrollbar::CVerticalScrollbar()
{
	m_Value = 0;
	m_ChangeSize = -1;
	m_Size = CSize();
}

void CVerticalScrollbar::SetPercent(double Percent)
{
	m_Value = Percent;
}

double CVerticalScrollbar::GetPercent()
{
	return m_Value;
}

void CVerticalScrollbar::SetChangeSize(double ChangeSize)
{
	m_ChangeSize = ChangeSize;
}

double CVerticalScrollbar::GetBarSize()
{
	if(m_ChangeSize <= 0)
		return 1;
	return m_ChangeSize * Size().Height;
}


void CVerticalScrollbar::Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin)
{
	CColour fgcol = Skin.ScrollForeground();
	CColour bgcol = Skin.ScrollBackground();

	if(HasFocus)
		fgcol = Skin.ScrollForegroundFocused();

	CSize selfsize = Size();

	Renderer.DrawBox(CVector(), selfsize, bgcol);
	
	double barsize =  Util::Round(GetBarSize(), 0.5);
	if(barsize < 0.5)
		barsize = 0.5;

	CVector pos = CVector(0, (selfsize.Height - barsize) * GetPercent());
	CSize size = CSize(selfsize.Width, barsize);

	//pos.Y = (int)pos.Y;

	char32_t tophalf = 0x2580;
	char32_t bottomhalf = 0x2584;
	char32_t full = 0x2588;

	if(!Renderer.SupportsUnicode())
	{
		tophalf = 223;
		bottomhalf = 220;
		full = 219;
	}

	for(double y = pos.Y; y < pos.Y + size.Height; y += 0.5)
	{
		std::unique_ptr<Console::ICharInformation> info = Renderer.GetCharInformation(CVector(0, y));
		char32_t cur = info->GetUnicodeChar();

		if(cur == tophalf) // if top half already,
		{
			if(fmod(y, 1.0) >= 0.5) // and we venture into lower, set to full
				cur = full;
		}
		else if(cur == bottomhalf) // if bottom half already,
		{
			if(fmod(y, 1.0) <= 0.5) // and we venture into upper, set to full
				cur = full;
		}
		else // otherwise
		{
			if(fmod(y, 1.0) < 0.5) // if we're in the top half, set it
				cur = tophalf;
			else // must be in the bottom, set that
				cur = bottomhalf;
		}

		info->SetUnicodeChar(cur);
		info->SetForegroundColour(fgcol);
	}

	//Renderer.DrawBox(pos, size, fgcol);

	return;

	if(fmod(pos.Y, 1.0) > 0.5)
	{
		pos.Y = (int)pos.Y;

		std::unique_ptr<Console::ICharInformation> infotop = Renderer.GetCharInformation(pos);
		std::unique_ptr<Console::ICharInformation> infobot = Renderer.GetCharInformation(pos + CVector(0, 1));

		infotop->SetChar((char)220);
		infobot->SetChar((char)223);

		infotop->SetForegroundColour(fgcol);
		infobot->SetForegroundColour(fgcol);
	}
	else
	{
		pos.Y = (int)pos.Y;
		std::unique_ptr<Console::ICharInformation> info = Renderer.GetCharInformation(pos);

		info->SetChar((char)219);
		info->SetForegroundColour(fgcol);
		//Renderer.DrawBox(pos, size, fgcol);
	}
}
