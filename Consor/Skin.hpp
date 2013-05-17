#ifndef CONSOR_SKIN_H
#define CONSOR_SKIN_H

#include "Units.hpp"
#include "ConsoleRenderer.hpp"
#include "Util/Debug.hpp"

namespace Consor
{
	class ISkin
	{
	protected:
		size_t _ColourPos;
	public:
		// returns the closest colour it can get
		virtual Colour RequestColour(Console::IConsoleRenderer& Renderer, const Colour& target)
		{
			size_t maxcols = Renderer.MaxColours();
			Colour* pColours = new Colour[maxcols];
			Renderer.GetColours(maxcols, pColours);
			

			if(_ColourPos >= maxcols) // search for an existing colour
			{
				for(size_t i = 0; i < maxcols; i++) // look for an exact match
				{
					if(pColours[i] == target)
					{
						delete [] pColours;
						return target;
					}
				}

				Colour closest = pColours[0];
				double closest_distance = Colour::Distance(target, closest);

				for(size_t i = 1; i < maxcols; i++)
				{
					double distance = Colour::Distance(target, pColours[i]);

					if(distance < closest_distance)
					{
						closest_distance = distance;
						closest = pColours[i];
					}
				}

				Util::Log("out of colours, using closest match % for % (dist: %)", closest, target, closest_distance);

				delete [] pColours;
				return closest;
			}

			for(size_t i = 0; i < _ColourPos; i++)
			{
				if(pColours[i] == target)
				{
					delete [] pColours;
					return target;
				}
			}

			// we didn't find the colour, create a new one...
			Util::Log("creating console colour #% %", _ColourPos, target);
			
			pColours[_ColourPos] = target;
			_ColourPos++;
			Renderer.SetColours(_ColourPos, pColours);

			delete [] pColours;
			return target;
		}

		virtual Colour Canvas() const = 0;

		virtual Colour LabelForeground() const = 0;
		virtual Colour LabelForegroundFocused() const = 0;

		virtual Colour WindowBorder() const = 0;
		virtual Colour WindowBackground() const = 0;
		virtual Colour WindowForeground() const = 0;
		virtual Colour WindowForegroundShine() const = 0;
		virtual char32_t WindowTitleLeftChar() const = 0;
		virtual char32_t WindowTitleRightChar() const = 0;

		virtual Colour ScrollForeground() const = 0;
		virtual Colour ScrollForegroundFocused() const = 0;
		virtual Colour ScrollBackground() const = 0;

		virtual Colour TextBoxForeground() const = 0;
		virtual Colour TextBoxForegroundFocused() const = 0;
		virtual Colour TextBoxBackground() const = 0;

		virtual Colour ProgressBarForeground(double Percent) const = 0;
		virtual Colour ProgressBarBackground(double Percent) const = 0;
		virtual Colour ProgressBarPercent(double Percent) const = 0;
	};

	class CDefaultSkin : public ISkin
	{
	protected:
		Colour CanvasColour;
		Colour Foreground;
		Colour White;
		Colour Black;
		Colour Background;
		Colour AlternateBackground;
		Colour FocusColour;
		Colour ForegroundShine;
		Colour ProgressPercent;
		Colour ProgressForeground;
		char32_t WindowLeft;
		char32_t WindowRight;

		CDefaultSkin()
		{
			WindowLeft = 196;
			WindowRight = 196;
		}
	public:
		CDefaultSkin(Console::IConsoleRenderer& Renderer)
		{
			if(!Renderer.SupportsUnicode())
			{
				WindowLeft = 196;
				WindowRight = 196;
			}
			else
			{
				WindowLeft = 0x2500;
				WindowRight = 0x2500;
			}

			_ColourPos = 0;
			CanvasColour = RequestColour(Renderer, Colour());
			Foreground = RequestColour(Renderer, Colour(1, 1, 1));
			ForegroundShine = RequestColour(Renderer, Colour(1, 1, 1));
			Background = RequestColour(Renderer, Colour(0, 0, 1));
			AlternateBackground = RequestColour(Renderer, Colour(0, 0, 0.5));
			FocusColour = RequestColour(Renderer, Colour(1, 0, 0));
			ProgressPercent = RequestColour(Renderer, Colour(1, 1, 1));
			ProgressForeground = RequestColour(Renderer, Colour(0, 1, 1));
		}

		virtual Colour Canvas() const
		{
			return CanvasColour;
		}

		virtual Colour LabelForeground() const
		{
			return Foreground;
		}

		virtual Colour LabelForegroundFocused() const
		{
			return FocusColour;
		}

		virtual Colour WindowBorder() const
		{
			return Foreground;
		}

		virtual Colour WindowBackground() const
		{
			return Background;
		}

		virtual Colour WindowForeground() const
		{
			return Foreground;
		}

		virtual Colour WindowForegroundShine() const
		{
			return ForegroundShine;
		}

		virtual char32_t WindowTitleLeftChar() const
		{
			return WindowLeft;
		}

		virtual char32_t WindowTitleRightChar() const
		{
			return WindowRight;
		}

		virtual Colour ScrollForeground() const
		{
			return Foreground;
		}

		virtual Colour ScrollForegroundFocused() const
		{
			return FocusColour;
		}	

		virtual Colour ScrollBackground() const
		{
			return AlternateBackground;
		}

		virtual Colour TextBoxForeground() const
		{
			return Foreground;
		}

		virtual Colour TextBoxForegroundFocused() const
		{
			return FocusColour;
		}

		virtual Colour TextBoxBackground() const
		{
			return Colour::None();
		}

		virtual Colour ProgressBarPercent(double Percent) const
		{
			return ProgressPercent;
		}

		virtual Colour ProgressBarForeground(double Percent) const
		{
			return ProgressForeground;
		}

		virtual Colour ProgressBarBackground(double Percent) const
		{
			return AlternateBackground;
		}
	};

	class CHackerSkin : public CDefaultSkin
	{
	public:
		CHackerSkin(Console::IConsoleRenderer& Renderer)
		{
			if(!Renderer.SupportsUnicode())
			{
				WindowLeft = 217;
				WindowRight = 192;

			}
			else
			{
				WindowLeft = 0x2518;
				WindowRight = 0x2514;
			}

			
			_ColourPos = 0;
			CanvasColour = RequestColour(Renderer, Colour());
			Foreground = RequestColour(Renderer, Colour(1, 0.5, 0));
			ForegroundShine = RequestColour(Renderer, Colour(1, 0.5, 0.25));
			Background = RequestColour(Renderer, Colour(0, 0, 0));
			AlternateBackground = RequestColour(Renderer, Colour(0.5, 0.25, 0));
			FocusColour = RequestColour(Renderer, Colour(1, 1, 1));
			ProgressPercent = RequestColour(Renderer, Colour(1, 1, 1));
			ProgressForeground = RequestColour(Renderer, Colour(1, 0.5, 0));
		};
	};
}

#endif