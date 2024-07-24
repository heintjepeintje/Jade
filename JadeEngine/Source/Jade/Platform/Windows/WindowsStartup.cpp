#ifdef _WIN32

#include "./../NativeStartup.hpp"
#include "./Windows.hpp"

#include "./WindowsWindow.hpp"
#include <cstdio>

namespace Jade {
	
	namespace Native {
	
		void _PlatformInit() {
			WNDCLASSA wc;
			ZeroMemory(&wc, sizeof(wc));
			
			wc.hCursor = LoadCursor(GetModuleHandle(NULL), IDC_ARROW);
			wc.hIcon = LoadCursor(GetModuleHandle(NULL), IDI_APPLICATION);
			wc.hInstance = GetModuleHandle(NULL);
			wc.lpfnWndProc = Windows::WindowProc;
			wc.lpszClassName = JD_WIN32_WINDOW_CLASS_NAME;
			
			JD_ASSERT(RegisterClassA(&wc) != 0, "Failed to register window class");
			
			WSADATA data;
			ZeroMemory(&data, sizeof(data));
			
			JD_ASSERT(WSAStartup(MAKEWORD(2, 2), &data) == 0, "Failed to startup Windows Sockets API (WSA)");
		}
		
		void _PlatformShutdown() {
			JD_ASSERT(WSACleanup() == 0, "Failed to cleanup Windows Sockets API (WSA)");
			JD_ASSERT(UnregisterClassA(JD_WIN32_WINDOW_CLASS_NAME, GetModuleHandle(NULL)), "Failed to unregister window class");
		}
	
	}
	
}

#endif // _WIN32