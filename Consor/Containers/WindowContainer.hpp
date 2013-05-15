#ifndef CONSOR_WINDOWCONTAINER_H
#define CONSOR_WINDOWCONTAINER_H

#include "../Control.hpp"
#include <string>

namespace Consor
{
	class CWindowContainer : public CControl
	{
	protected:
		CControl* m_pClient;
		std::string m_Title;
		CWindowContainer();
	public:
		CWindowContainer(CControl& Client, const std::string& Title);
		virtual CSize Size();
		virtual void OnResize(const CSize& Size);
		virtual void ForceResize(const CSize& Size);
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
		virtual bool HandleInput(Input::Key Key, Input::IInputSystem& System);
		virtual bool CanFocus();
	};
};

#endif