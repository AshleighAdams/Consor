#include <iostream>

#include "WindowsConsoleRenderer.hpp"
#include "Util/StringUtils.hpp"
#include "Util/Time.hpp"

using namespace std;

int main(int count, char** values)
{
	Consor::Console::CWindowsConsoleRenderer renderer; 

	Consor::CSize consolesize = renderer.RenderSize();;
	renderer.PushRenderBounds(Consor::CVector(5, 5), consolesize - Consor::CSize(5, 5) * 2);
	
	Consor::CSize bounds = renderer.RenderSize();

	size_t height;
	string input = "Lorem ipsum dolor sit amet, poop consectetur adipiscing elit. Pellentesque auctor tincidunt justo eu convallis. Integer fringilla condimentum dolor vitae tincidunt. Nunc vehicula elementum odio, condimentum scelerisque odio cursus id. Pellentesque ac quam nunc. Sed vitae iaculis dui. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Duis faucibus placerat augue iaculis cursus. Suspendisse id dolor auctor leo tristique tincidunt. Maecenas adipiscing imperdiet eros, ac pulvinar dui congue ut. Nulla facilisi. Nam ornare, felis et accumsan porttitor, dui velit luctus augue, nec dictum quam eros et risus. Phasellus convallis, nunc eget fringilla eleifend, nibh sapien posuere enim, ac ultrices mauris lorem in mauris. Sed a augue vel est convallis facilisis nec ut nisl.";
	string trimmed = Consor::Util::WrapText(input, bounds.Width - 1, nullptr, &height);

	while(true)
	{
		double start = Consor::Util::GetTime();

		renderer.Clear(Consor::CColour());
		
		renderer.DrawRect(Consor::CVector(), Consor::CSize(bounds.Width, height + 1), Consor::CColour(1,1,1), Consor::CColour());
		//renderer.DrawString(trimmed, Consor::CVector(1, 1), Consor::CColour(1, 1, 1), Consor::CColour(0, 0, 0, 0));

		double end = Consor::Util::GetTime();
		double span = end - start;

		renderer.DrawString(Consor::Util::FormatString("FPS: %   ", 1.0/span), Consor::CVector(2,2), Consor::CColour(1, 1, 1), Consor::CColour(0, 0, 0, 0));

		renderer.FlushToScreen();
	}

	renderer.PopRenderBounds();

	return 0;
}