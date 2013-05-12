#ifndef CONSOR_SKIN_H
#define CONSOR_SKIN_H

#include "Units.hpp"
#include "ConsoleRenderer.hpp"

namespace Consor
{
	class ISkin
	{
	public:
		virtual void SetRendererColours(Console::IConsoleRenderer& Renderer) = 0;

		virtual CColour LabelForeground() const = 0;
		virtual CColour LabelForegroundFocused() const = 0;

		virtual CColour WindowBorder() const = 0;
		virtual CColour WindowBackground() const = 0;
		virtual CColour WindowForeground() const = 0;

		virtual CColour ScrollForeground() const = 0;
		virtual CColour ScrollForegroundFocused() const = 0;
		virtual CColour ScrollBackground() const = 0;

		virtual CColour TextBoxForeground() const = 0;
		virtual CColour TextBoxForegroundFocused() const = 0;
		virtual CColour TextBoxBackground() const = 0;
	};

	class CDefaultSkin : public ISkin
	{
		int TotalColours;
		CColour Foreground;
		CColour White;
		CColour Black;
		CColour Background;
		CColour AlternateBackground;
		CColour FocusColour;
	public:
		CDefaultSkin()
		{
			Foreground = CColour(1, 0.5, 0);
			White = CColour(1, 1, 1);
			Black = CColour();
			Background = Black;
			AlternateBackground = CColour(0.25, 0.25*0.5, 0);
			FocusColour = CColour(1, 0, 0);
		}

		virtual void SetRendererColours(Console::IConsoleRenderer& Renderer)
		{
			const int TotalColours = 6;
			size_t supported_colours = Renderer.MaxColours();

			if(supported_colours < TotalColours)
				throw std::exception("Not enough available colours for this skin");

			CColour colours[TotalColours];
			Renderer.GetColours(TotalColours, colours);
			{
				colours[0] = Foreground;
				colours[1] = White;
				colours[2] = Black;
				colours[3] = Background;
				colours[4] = AlternateBackground;
				colours[5] = FocusColour;
			}
			Renderer.SetColours(TotalColours, colours);
		}

		virtual CColour LabelForeground() const
		{
			return Foreground;
		}

		virtual CColour LabelForegroundFocused() const
		{
			return FocusColour;
		}

		virtual CColour WindowBorder() const
		{
			return Foreground;
		}

		virtual CColour WindowBackground() const
		{
			return Background;
		}

		virtual CColour WindowForeground() const
		{
			return Foreground;
		}

		virtual CColour ScrollForeground() const
		{
			return Foreground;
		}

		virtual CColour ScrollForegroundFocused() const
		{
			return FocusColour;
		}	

		virtual CColour ScrollBackground() const
		{
			return AlternateBackground;
		}

		virtual CColour TextBoxForeground() const
		{
			return Foreground;
		}

		virtual CColour TextBoxForegroundFocused() const
		{
			return FocusColour;
		}

		virtual CColour TextBoxBackground() const
		{
			return CColour::None();
		}
	};
}

#endif