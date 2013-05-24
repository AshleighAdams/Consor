#ifndef CONSOR_ANSICONSOLERENDERER_H
#define CONSOR_ANSICONSOLERENDERER_H

#include "ConsoleRenderer.hpp"
#include <iostream>
#include <ostream>

#define ANSI_MAX_COLOURS 256 // providing they're on xterm, this should work

namespace Consor
{
	namespace Console
	{
		namespace ANSI
		{
			static const char CSI = '\x1B';

			template<typename T>
			inline void _GenerateEscapeSequence(std::ostream& os, T arg) // final
			{
				os << arg; // notice how no ;
			}

			template<typename T, typename... Args>
			inline void _GenerateEscapeSequence(std::ostream& os, T arg, Args... args) // itterator
			{
				os << arg << ';';
				_GenerateEscapeSequence(os, args...);
			}

			template<typename... Args>
			inline void GenerateEscapeSequence(std::ostream& os, char bracket, char seq, Args... args) // starter
			{
				os << CSI << bracket;
				_GenerateEscapeSequence(os, args...);
				os << (char)seq;
			}
			
			enum EscapeSequence // not all, only ones useful to us
			{
				CursorUp = 'A',
				CursorDown = 'B',
				CursorForward = 'C',
				CursorBack = 'D',
				CursorNextLine = 'E',
				CursorPrevLine = 'F',
				CursorPosition = 'H',
				HorizontalAndVerticalPosition = 'f',
				SelectGraphicRendition = 'm'
			};
			
			namespace GraphicRendition // not all of them, again
			{
				static std::string Reset = "0";
				static std::string Bold = "1";
				static std::string UnderlineSingle = "4";
				static std::string UnderlineNone = "24";
				static std::string BlinkSlow = "5";
				static std::string BlinkRapid = "6";
				static std::string BlinkOff = "25";
				static std::string Reverse = "7";
				static std::string DefaultFont = "10";
				
				static std::string TextColour = "30"; // to 37, for each of the 8 system colours
				static std::string TextColour256 = "38"; // 5; COLOUR
				static std::string TextColourDefault = "39";
				
				static std::string BackgroundColour = "40"; // + 7 for each of the system colours
				static std::string BackgroundColour256 = "48";
				static std::string BackgroundColourDefault = "49";
			};
		};
		
		struct ANSI_CHAR_INFO
		{
			char32_t Letter;
			Colour BG;
			Colour FG;
		};
		
		class ANSIConsoleRenderer;
		
		class ANSICharInformation : public ICharInformation
		{
			ANSIConsoleRenderer* _pRenderer;
			Vector _Position;
		public:
			ANSICharInformation(ANSIConsoleRenderer& Parent, const Vector& pos);
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
		
		class ANSIConsoleRenderer : public IConsoleRenderer
		{
			friend class ANSICharInformation;
			
			bool _SupportsUTF8;
			size_t _Width, _Height;
			void _CheckConsoleSize(); // this will resize and handle buffer changes, if something changed
			
			bool _CompleteFlush; // instruct the renderer to completley flush the screen
			ANSI_CHAR_INFO* _pBuffer;
			Colour _ColourTable[ANSI_MAX_COLOURS];
			Colour _OriginalColourTable[ANSI_MAX_COLOURS];
			ANSI_CHAR_INFO& _GetCharInfo(const Vector& vec);
			size_t _ColourToColourIndex(const Colour& col);
			// this func returns: closest colour, exact match
			std::tuple<Colour, bool> _ClosestColourMatch(const Colour& col, bool OnlyCreated = false);
		public:
			ANSIConsoleRenderer();
			~ANSIConsoleRenderer();

			// platform specific stuff
			std::string RendererName();
			std::string VersionString();

			void FlushToScreen();
			std::unique_ptr<ICharInformation> GetCharInformation(const Vector& pos);
			Size GetSize();

			bool SupportsUnicode();
			// this will probably replace existing colours
			size_t MaxColours();
			void GetColours(size_t Count, Colour* pColours);
			void SetColours(size_t Count, Colour* pColours);
			void ResetColours();

			void SetTitle(const std::string& Title);
		};
		
	};
};

#endif
