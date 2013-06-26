#ifndef CONSOR_BORDERCONTAINER_H
#define CONSOR_BORDERCONTAINER_H

#include "../Control.hpp"
#include <string>

namespace Consor
{
	class BorderContainer : public Control
	{
	protected:
		Control* _pClient;
		size_t _BorderLeft;
		size_t _BorderRight;
		size_t _BorderTop;
		size_t _BorderBottom;
		void _SetBorder(size_t Left, size_t Right, size_t Top, size_t Bottom);
	public:
		BorderContainer(Control& Client, size_t Border);
		BorderContainer(Control& Client, size_t Left, size_t Right, size_t Top, size_t Bottom);
		virtual Size GetSize();
		virtual void OnResize(const Size& Size);
		virtual void ForceResize(const Size& Size);
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
		virtual bool HandleInput(Input::Key Key, Input::IInputSystem& System);
		virtual bool CanFocus();
	};
};

#endif