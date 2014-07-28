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
			std::list<renderbound_t> _Bounds, _BoundsExposed;
			std::list<Vector> _Offsets;
			Vector _CurrentOffset;
			renderbound_t _CurrentRenderBound;

			size_t _CurrentColour;
			bool _FlushColours;
			bool _NoPopAssert;
			std::list<Colour> _NewColours;
			virtual std::tuple<Colour, bool> _ClosestColourMatch(const Colour& target, bool only_created = false) = 0;
		public:
			virtual ~IConsoleRenderer(){}

			// platform specific stuff
			/// \return The renderer's name.
			virtual std::string RendererName();
			/// \return Information about the renderer.  Such as: `"built Jul 18 2014 23:00:36; abstract renderer; impliments and supports: 256 custom colours, unicode; direct draw calls: none; abstract draw calls: DrawBox, DrawRect, DrawString; flushmode: all; built with: GNU GCC/G++ 4.9.0; C++ version: 201103; bits: 64;"`
			virtual std::string VersionString() = 0;

			/// Write the UI to the console.
			virtual void FlushToScreen() = 0;
			/// Get information about a specific char at the specified position.
			virtual std::unique_ptr<ICharInformation> GetCharInformation(const Vector& pos) = 0;
			/// \return Size of the console.
			virtual Size GetSize() = 0;

			/// \return If the console supports Unicode, `true`; otherwise `false`.
			virtual bool SupportsUnicode() = 0;
			/// \return Number of colours supported by the console.
			virtual size_t MaxColours() = 0;
			/// Fill the array `pColours` with the current console colours.
			/// \param Count The size of our array to prevent buffer overflows, should be equal to `this->MaxColours()`.
			virtual void GetColours(size_t Count, Colour* pColours) = 0;
			/// Set the console colours from the array `pColours`.
			/// \param Count The size of our array to prevent buffer overflows, should be equal to `this->MaxColours()`.
			/// \warn If you wish to create a colour, you should use `RequestColour`!
			virtual void SetColours(size_t Count, Colour* pColours) = 0;
			/// Reset the colours to the console's defaults.
			virtual void ResetColours() = 0;
			// this function is a helper which uses the ones above, if you directly modify the colour table
			// don't be supprised if bugs arise
			/// Get the closest colour possible.  Will create it if `make == true`.
			virtual Colour RequestColour(const Colour& target, bool make = false);
			/// When colours have been modified, this should be called.
			/// \warn It is the console renderer's job to call this.
			virtual void FlushRequestedColours();

			/// Set the console's title.
			/// \param Title The string to set the title to.
			virtual void SetTitle(const std::string& Title) = 0;

			// other stuff
			/// Clears the console.
			/// \param col The colour used to clear.
			virtual void Clear(const Colour& col);
			
			/// Draws a filled box.
			virtual void DrawBox(const Vector& pos, const Size& size, const Colour& col);
			/// Draws a rectangle with the specified foreground and background colours.
			virtual void DrawRect(const Vector& pos, const Size& size, const Colour& fg_col, const Colour& bg_col);
			virtual void DrawString(const std::string& str, const Vector& pos, const Colour& fgcol, const Colour& bgcol);
			
			/// Draw functions will see 0, 0 at the position `from`, and can draw to `size`.  Any drawing outside of that range is clipped.
			virtual void PushRenderBounds(const Vector& from, const Consor::Size& size);
			/// Pop a render bound off of the stack.
			virtual void PopRenderBounds();
			/// The available space to render our content in.
			virtual Size RenderSize();
			/// \return The absolute position of the current render bounds.
			virtual Vector RenderOffset();
			/// Is the position inside of the render bounds?
			/// \param dir The direction it is out of bounds in.
			virtual bool InRenderBounds(const Vector& pos, Vector* dir);
			/// Is the box inside of the render bounds?
			virtual bool InRenderBounds(const Vector& pos, const Consor::Size& size);
		};
	}
}

#endif
