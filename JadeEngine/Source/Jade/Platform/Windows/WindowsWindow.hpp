#pragma once

#ifdef _WIN32

#include "./Windows.hpp"

namespace Jade {

	namespace Windows {
	
		LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wp, LPARAM lp);
	
	}

}

#endif // _WIN32