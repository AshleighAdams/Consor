
#if defined(_WIN32) || defined(_WIN64)

#include "WindowsInputSystem.hpp"
namespace Consor { namespace Input { typedef Consor::Input::WindowsInputSystem PlatformInputSystem; } }

#else // probably Linux

#include "LinuxInputSystem.hpp"
namespace Consor { namespace Input { typedef Consor::Input::LinuxInputSystem PlatformInputSystem; } }

#endif
