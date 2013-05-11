#ifndef CONSOR_SKIN_H
#define CONSOR_SKIN_H

#include "Units.hpp"

namespace Consor
{
	class ISkin
	{
	public:
		virtual CColour LableForeground() const = 0;
		virtual CColour LableForegroundFocused() const = 0;
		virtual CColour WindowBorder() const = 0;
		virtual CColour WindowBackground() const = 0;
		virtual CColour WindowForeground() const = 0;

		virtual CColour ScrollForeground() const = 0;
		virtual CColour ScrollBackground() const = 0;
	};

	class CDefaultSkin : public ISkin
	{
	public:
		CDefaultSkin()
		{
		}

		virtual CColour LableForeground() const
		{
			return CColour(1, 1, 1);
		}

		virtual CColour LableForegroundFocused() const
		{
			return CColour(1, 0, 0);
		}

		virtual CColour WindowBorder() const
		{
			return CColour(1, 1, 1);
		}

		virtual CColour WindowBackground() const
		{
			return CColour(0, 0, 1);
		}

		virtual CColour WindowForeground() const
		{
			return CColour(1, 1, 1);
		}

		virtual CColour ScrollForeground() const
		{
			return CColour(1, 1, 1);
		}

		virtual CColour ScrollBackground() const
		{
			return CColour(0, 0, 0.5);
		}
	};
}

#endif