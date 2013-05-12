#ifndef WINDOWS_CONSOLE_RENDERER_H
#define WINDOWS_CONSOLE_RENDERER_H

#include "ConsoleRenderer.hpp"

#include <memory>
#include <list>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// not having to go through all the hoops of ICharInfo significantly speeds up the rendering process
//#define WINDOWS_CONSOLE_RENDERER_FAST

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

		class CWindowsConsoleRenderer;

		class CWindowsCharInformation : public ICharInformation
		{
			CWindowsConsoleRenderer* m_pRenderer;
			CVector m_Position;
		public:
			CWindowsCharInformation(CWindowsConsoleRenderer* pRenderer, CVector pos);
			void SetAttributes(CharAttributes attr);
			CharAttributes GetAttributes();
			
			void SetForegroundColour(const CColour& col);
			CColour GetForegroundColour();

			void SetBackgroundColour(const CColour& col);
			CColour GetBackgroundColour();

			void SetChar(char val);
			char GetChar();

			bool SupportsUnicode();
			void SetUnicodeChar(char32_t val);
			char32_t GetUnicodeChar();
		};

		class CWindowsConsoleRenderer : public IConsoleRenderer
		{
			friend class CWindowsCharInformation;

//#ifdef WINDOWS_CONSOLERENDERER_FRIENDS
		public:
//#endif

			HANDLE          m_BufferHandle;
			HANDLE          m_STDOutHandle;
			int             m_Width;
			int             m_Height;
			CHAR_INFO*      m_pBuffer;

			CHAR_INFO& _CharInfoAt(int x, int y);

			CColour         m_ColourTable[16];
		public:
			CWindowsConsoleRenderer();
			~CWindowsConsoleRenderer();

			std::unique_ptr<ICharInformation> GetCharInformation(const CVector& pos);
			void FlushToScreen();
			CSize Size();

			size_t MaxColours();
			void GetColours(size_t Count, CColour* pColours);
			void SetColours(size_t Count, CColour* pColours);


#ifdef WINDOWS_CONSOLE_RENDERER_FAST
			void DrawBox(const CVector& pos, const CSize& size, const CColour& col);
			void DrawRect(const CVector& pos, const CSize& size, const CColour& fgcol, const CColour& bgcol);
			void DrawString(const std::string& str, const CVector& pos, const CColour& fgcol, const CColour& bgcol);
#endif
		};
	};
};
#endif