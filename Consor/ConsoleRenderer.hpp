#ifndef CONSOLE_RENDERER_H
#define CONSOLE_RENDERER_H

#ifndef _MSC_VER
#include <uchar.h>
#endif

#include "Units.hpp"
#include <string>
#include <memory>
#include <list>

namespace Consor
{
	namespace Console
	{
		class ICharInformation
		{
		public:
			virtual ~ICharInformation() {}
			virtual void SetPosition(const Vector& pos) = 0;
			virtual void SetForegroundColour(const Colour& col) = 0;
			virtual Colour GetForegroundColour() = 0;

			virtual void SetBackgroundColour(const Colour& col) = 0;
			virtual Colour GetBackgroundColour() = 0;

			virtual void SetChar(char val) = 0;
			virtual char GetChar() = 0;

			virtual bool SupportsUnicode() = 0;
			virtual void SetUnicodeChar(char32_t val) = 0;
			virtual char32_t GetUnicodeChar() = 0;

		};

		struct renderbound_t
		{
			Vector pos;
			Size size;
		};

		class IConsoleRenderer
		{
		protected:
			std::list<renderbound_t> _Bounds;
			std::list<Vector> _Offsets;
			Vector _CurrentOffset;
			renderbound_t _CurrentRenderBound;

			size_t _CurrentColour;
			bool _FlushColours;
			std::list<Colour> _NewColours;
			virtual std::tuple<Colour, bool> _ClosestColourMatch(const Colour& target, bool only_created = false) = 0;
		public:
			virtual ~IConsoleRenderer(){}

			// platform specific stuff
			virtual std::string RendererName();
			virtual std::string VersionString() = 0;

			virtual void FlushToScreen() = 0;
			virtual std::unique_ptr<ICharInformation> GetCharInformation(const Vector& pos) = 0;
			virtual Size GetSize() = 0;

			virtual bool SupportsUnicode() = 0;
			// this will probably replace existing colours
			virtual size_t MaxColours() = 0;
			virtual void GetColours(size_t Count, Colour* pColours) = 0;
			virtual void SetColours(size_t Count, Colour* pColours) = 0;
			virtual void ResetColours() = 0;
			// this function is a helper which uses the ones above, if you directly modify the colour table
			// don't be supprised if bugs arise
			virtual Colour RequestColour(const Colour& target, bool make = false);
			virtual void FlushRequestedColours();

			virtual void SetTitle(const std::string& Title) = 0;

			// other stuff
			virtual void Clear(const Colour& col);

			virtual void DrawBox(const Vector& pos, const Size& size, const Colour& col);
			virtual void DrawRect(const Vector& pos, const Size& size, const Colour& fg_col, const Colour& bg_col);
			virtual void DrawString(const std::string& str, const Vector& pos, const Colour& fgcol, const Colour& bgcol);
			
			virtual void PushRenderBounds(const Vector& from, const Consor::Size& size);
			virtual void PopRenderBounds();
			virtual Size RenderSize();
			virtual bool InRenderBounds(const Vector& pos, Vector* dir);
			virtual bool InRenderBounds(const Vector& pos, const Consor::Size& size);
		};
	}
}

#endif
