#ifndef CONSOLE_RENDERER_H
#define CONSOLE_RENDERER_H

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
			virtual void SetForegroundColour(const CColour& col) = 0;
			virtual CColour GetForegroundColour() = 0;

			virtual void SetBackgroundColour(const CColour& col) = 0;
			virtual CColour GetBackgroundColour() = 0;

			virtual void SetChar(char val) = 0;
			virtual char GetChar() = 0;

			virtual bool SupportsUnicode() = 0;
			virtual void SetUnicodeChar(char32_t val) = 0;
			virtual char32_t GetUnicodeChar() = 0;

		};

		struct renderbound_t
		{
			CVector Pos;
			CSize Size;
		};

		class IConsoleRenderer
		{
		protected:
			std::list<renderbound_t> m_Bounds;
			std::list<CVector> m_Offsets;
			CVector m_CurrentOffset;
			renderbound_t m_CurrentRenderBound;
		public:
			// platform specific stuff
			virtual std::string RendererName();
			virtual std::string VersionString() = 0;

			virtual void FlushToScreen() = 0;
			virtual std::unique_ptr<ICharInformation> GetCharInformation(const CVector& pos) = 0;
			virtual CSize Size() = 0;

			// this will probably replace existing colours
			virtual size_t MaxColours() = 0;
			virtual void GetColours(size_t Count, CColour* pColours) = 0;
			virtual void SetColours(size_t Count, CColour* pColours) = 0;
			virtual void ResetColours() = 0;

			virtual void SetTitle(const std::string& Title) = 0;

			// other stuff
			virtual ~IConsoleRenderer() {}
			virtual void Clear(const CColour& col);

			virtual void DrawBox(const CVector& pos, const CSize& size, const CColour& col);
			virtual void DrawRect(const CVector& pos, const CSize& size, const CColour& fg_col, const CColour& bg_col);
			virtual void DrawString(const std::string& str, const CVector& pos, const CColour& fgcol, const CColour& bgcol);
			
			virtual void PushRenderBounds(const CVector& from, const CSize& size);
			virtual void PopRenderBounds();
			virtual CSize RenderSize();
		};
	}
}

#endif