#ifndef CONSOR_BORDERCONTAINER_H
#define CONSOR_BORDERCONTAINER_H

#include "Control.hpp"
#include <string>

namespace Consor
{
	class CBorderContainer : public CControl
	{
	protected:
		CControl* m_pClient;
		size_t m_BorderLeft;
		size_t m_BorderRight;
		size_t m_BorderTop;
		size_t m_BorderBottom;
		void m_SetBorder(size_t Left, size_t Right, size_t Top, size_t Bottom);
	public:
		CBorderContainer(CControl& Client, size_t Border);
		CBorderContainer(CControl& Client, size_t Left, size_t Right, size_t Top, size_t Bottom);
		virtual CSize Size();
		virtual void OnResize(const CSize& Size);
		virtual void ForceResize(const CSize& Size);
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
		virtual bool HandleInput(Input::Key Key);
		virtual bool CanFocus();
	};
};

#endif