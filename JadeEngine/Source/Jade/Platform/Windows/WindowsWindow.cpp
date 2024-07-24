#ifdef _WIN32

#include "./WindowsWindow.hpp"

namespace Jade {

	namespace Windows {
	
		LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wp, LPARAM lp) {
			return DefWindowProcA(window, message, wp, lp);
		}
	
	}

}

#endif // _WIN32