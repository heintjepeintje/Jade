#pragma once

#if defined(_WIN32)

#include "./../../System/Input.hpp"
#include "./../../Core.hpp"

namespace Jade {
	
	namespace Windows {
		
		int ToWindowsKey(KeyCode key);
		KeyCode ToJadeKey(int key);
		
		int ToWindowsMouseButton(MouseButton button);
		MouseButton ToJadeMouseButton(int button);
		
	}
	
}

#endif