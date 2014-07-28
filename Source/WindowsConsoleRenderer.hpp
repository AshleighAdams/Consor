#ifndef WINDOWS_CONSOLE_RENDERER_H
#define WINDOWS_CONSOLE_RENDERER_H

#include "ConsoleRenderer.hpp"

#include <memory>
#include <list>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// not having to go through all the hoops of ICharInfo significantly speeds up the rendering process
#define WINDOWS_CONSOLE_RENDERER_FAST

namespace Consor
{
	namespace Console
	{
		enum CharAttributes : int
		{
			Ignore = -1,
			None = 0,
			ForegroundBlue = 0x1,
			ForegroundGreen = 0x2,
			ForegroundRed = 0x4,
			ForegroundIntense = 0x8,
			BackgroundBlue = 0x10,
			BackgroundGreen = 0x20,
			BackgroundRed = 0x40,
			BackgroundIntense = 0x80,
			ReverseColours = 0x4000,
			Underscore = 0x8000,

			// helpers: foreground
			ForegroundGrey = ForegroundRed | ForegroundGreen | ForegroundBlue,
			ForegroundWhite = ForegroundGrey | ForegroundIntense,
			ForegroundCyan = ForegroundGreen | ForegroundBlue,
			ForegroundMagenta = ForegroundRed | ForegroundBlue,
			ForegroundYellow = ForegroundRed | ForegroundGreen,
		
			// helpers: background
			BackgroundGrey = BackgroundRed | BackgroundGreen | BackgroundBlue,
			BackgroundWhite = BackgroundGrey | BackgroundIntense,
			BackgroundCyan = BackgroundGreen | BackgroundBlue,
			BackgroundMagenta = BackgroundRed | BackgroundBlue,
			BackgroundYellow = BackgroundRed | BackgroundGreen
		};

#define FIX_OPERATOR(type, op) \
		inline type operator op(const type& a, const type& b)\
		{\
			return (type)((int)a op (int)b);\
		}\

		FIX_OPERATOR(CharAttributes, |);
		FIX_OPERATOR(CharAttributes, -);
		FIX_OPERATOR(CharAttributes, +);
#undef FIX_OPERATOR

		class WindowsConsoleRenderer;

		class CWindowsCharInformation : public ICharInformation
		{
			WindowsConsoleRenderer* _pRenderer;
			Vector _Position;
		public:
			CWindowsCharInformation(WindowsConsoleRenderer* pRenderer, Vector pos);
			void SetAttributes(CharAttributes attr);
			CharAttributes GetAttributes();
			void SetPosition(const Vector& pos);

			void SetForegroundColour(const Colour& col);
			Colour GetForegroundColour();

			void SetBackgroundColour(const Colour& col);
			Colour GetBackgroundColour();

			void SetChar(char val);
			char GetChar();

			bool SupportsUnicode();
			void SetUnicodeChar(char32_t val);
			char32_t GetUnicodeChar();
		};

		/// A console renderer implimentation that uses Window's API.
		class WindowsConsoleRenderer : public IConsoleRenderer
		{
			friend class CWindowsCharInformation;

//#ifdef WINDOWS_CONSOLERENDERER_FRIENDS
		public:
//#endif

			HANDLE          _BufferHandle;
			HANDLE          _STDOutHandle;
			size_t          _Width;
			size_t          _Height;
			CHAR_INFO*      _pBuffer;
			CHAR_INFO*      _pBufferDelta;
			bool            _WroteOnce;

			CHAR_INFO& _CharInfoAt(int x, int y);

			Colour         _ColourTable[16];
			Colour         _OriginalColourTable[16];
			std::tuple<Colour, bool> _ClosestColourMatch(const Colour& target, bool only_created = false);
		public:
			WindowsConsoleRenderer();
			~WindowsConsoleRenderer();

			std::string RendererName();
			std::string VersionString();

			std::unique_ptr<ICharInformation> GetCharInformation(const Vector& pos);
			void FlushToScreen();
			Size GetSize();

			bool SupportsUnicode();
			size_t MaxColours();
			void GetColours(size_t Count, Colour* pColours);
			void SetColours(size_t Count, Colour* pColours);
			void ResetColours();

			void SetTitle(const std::string& Title);
#ifdef WINDOWS_CONSOLE_RENDERER_FAST
			void DrawBox(const Vector& pos, const Size& size, const Colour& col);
			//leave DrawRect to the abstract renderer, it's way, way too messy for us to keep it up to date here
			//void DrawRect(const Vector& pos, const Size& size, const Colour& fgcol, const Colour& bgcol);
			void DrawString(const std::string& str, const Vector& pos, const Colour& fgcol, const Colour& bgcol);
#endif
		};
	};
};
#endif
