#ifndef CONSOR_BORDERCONTAINER_H
#define CONSOR_BORDERCONTAINER_H

#include "Control.hpp"
#include <string>

namespace Consor
{
	class CWindowsContainer : public CControl
	{
	protected:
		CControl* m_pClient;
		std::string m_Title;
	public:
		CWindowsContainer(CControl& Client, const std::string& Title);
		virtual CSize Size();
		virtual void OnResize(const CSize& Size);
		virtual void ForceResize(const CSize& Size);
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
		virtual bool HandleInput(int Key);
		virtual bool CanFocus();
	};
};

#endif