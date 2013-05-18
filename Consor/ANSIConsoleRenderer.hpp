#ifndef CONSOR_ANSICONSOLERENDERER_H
#define CONSOR_ANSICONSOLERENDERER_H

#include "ConsoleRenderer.hpp"

#define ANSI_MAX_COLOURS 8

namespace Consor
{
	namespace Console
	{
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
			
			size_t _Width, _Height;
			ANSI_CHAR_INFO* _pBuffer;
			Colour _ColourTable[ANSI_MAX_COLOURS];
			Colour _OriginalTable[ANSI_MAX_COLOURS];
			ANSI_CHAR_INFO& _GetCharInfo(const Vector& vec);
		public:
			ANSIConsoleRenderer();
			virtual ~ANSIConsoleRenderer(){}

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
