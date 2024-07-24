#pragma once

#ifdef _WIN32

#include <Jade/Core.hpp>

#include <windef.h>
#include <winbase.h>
#include <winuser.h>
#include <windowsx.h>
#include <ws2tcpip.h>

#define JD_WIN32_WINDOW_CLASS_NAME "JDWNDCLASS"

#endif // _WIN32