
#if defined(_WIN32) || defined(_WIN64)

#include "WindowsConsoleRenderer.hpp"
namespace Consor { namespace Console { typedef Consor::Console::WindowsConsoleRenderer PlatformConsoleRenderer; } }

#else // probably Linux

#include "ANSIConsoleRenderer.hpp"
namespace Consor { namespace Console { typedef Consor::Console::ANSIConsoleRenderer PlatformConsoleRenderer; } }

#endif
