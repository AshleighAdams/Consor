#ifndef CONTROLS_LABLE_H
#define CONTROLS_LABLE_H

#include "../Control.hpp"
#include <string>

namespace Consor
{
	class Label : public Control
	{
	protected:
		std::string _Text;
		std::string _SetText;
	public:
		Label();
		void SetText(const std::string& Text);
		virtual void Draw(Consor::Console::IConsoleRenderer& Renderer, bool HasFocus, const Consor::ISkin& Skin);
		virtual void ForceResize(const Size& Size);
	};
}

#endif