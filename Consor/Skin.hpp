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
		size_t m_ColourPos;
	public:
		// returns the closest colour it can get
		virtual CColour RequestColour(Console::IConsoleRenderer& Renderer, const CColour& target)
		{
			size_t maxcols = Renderer.MaxColours();
			CColour* pColours = new CColour[maxcols];
			Renderer.GetColours(maxcols, pColours);
			

			if(m_ColourPos >= maxcols) // search for an existing colour
			{
				for(size_t i = 0; i < maxcols; i++) // look for an exact match
				{
					if(pColours[i] == target)
					{
						delete [] pColours;
						return target;
					}
				}

				CColour closest = pColours[0];
				double closest_distance = CColour::Distance(target, closest);

				for(size_t i = 1; i < maxcols; i++)
				{
					double distance = CColour::Distance(target, pColours[i]);

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

			for(size_t i = 0; i < m_ColourPos; i++)
			{
				if(pColours[i] == target)
				{
					delete [] pColours;
					return target;
				}
			}

			// we didn't find the colour, create a new one...
			Util::Log("creating console colour #% %", m_ColourPos, target);
			
			pColours[m_ColourPos] = target;
			m_ColourPos++;
			Renderer.SetColours(m_ColourPos, pColours);

			delete [] pColours;
			return target;
		}

		virtual CColour LabelForeground() const = 0;
		virtual CColour LabelForegroundFocused() const = 0;

		virtual CColour WindowBorder() const = 0;
		virtual CColour WindowBackground() const = 0;
		virtual CColour WindowForeground() const = 0;
		virtual CColour WindowForegroundShine() const = 0;
		virtual char32_t WindowTitleLeftChar() const = 0;
		virtual char32_t WindowTitleRightChar() const = 0;

		virtual CColour ScrollForeground() const = 0;
		virtual CColour ScrollForegroundFocused() const = 0;
		virtual CColour ScrollBackground() const = 0;

		virtual CColour TextBoxForeground() const = 0;
		virtual CColour TextBoxForegroundFocused() const = 0;
		virtual CColour TextBoxBackground() const = 0;

		virtual CColour ProgressBarForeground(double Percent) const = 0;
		virtual CColour ProgressBarBackground(double Percent) const = 0;
		virtual CColour ProgressBarPercent(double Percent) const = 0;
	};

	class CDefaultSkin : public ISkin
	{
	protected:
		CColour Foreground;
		CColour White;
		CColour Black;
		CColour Background;
		CColour AlternateBackground;
		CColour FocusColour;
		CColour ForegroundShine;
		CColour ProgressPercent;
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
			WindowLeft = 196;
			WindowRight = 196;

			m_ColourPos = 0;
			Foreground = RequestColour(Renderer, CColour(1, 1, 1));
			ForegroundShine = RequestColour(Renderer, CColour(1, 1, 1));
			Background = RequestColour(Renderer, CColour(0, 0, 1));
			AlternateBackground = RequestColour(Renderer, CColour(0, 0, 0.5));
			FocusColour = RequestColour(Renderer, CColour(1, 0, 0));
			ProgressPercent = RequestColour(Renderer, CColour(1, 1, 1));
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

		virtual CColour WindowForegroundShine() const
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

		virtual CColour ProgressBarPercent(double Percent) const
		{
			return ProgressPercent;
		}

		virtual CColour ProgressBarForeground(double Percent) const
		{
			return Foreground;
		}

		virtual CColour ProgressBarBackground(double Percent) const
		{
			return AlternateBackground;
		}
	};

	class CHackerSkin : public CDefaultSkin
	{
	public:
		CHackerSkin(Console::IConsoleRenderer& Renderer)
		{
			WindowLeft = 217;
			WindowRight = 192;

			m_ColourPos = 0;
			Foreground = RequestColour(Renderer, CColour(1, 0.5, 0));
			ForegroundShine = RequestColour(Renderer, CColour(1, 0.5, 0.25));
			Background = RequestColour(Renderer, CColour(0, 0, 0));
			AlternateBackground = RequestColour(Renderer, CColour(0.5, 0.25, 0));
			FocusColour = RequestColour(Renderer, CColour(1, 0, 0));
			ProgressPercent = RequestColour(Renderer, CColour(1, 1, 1));
		};
	};
}

#endif