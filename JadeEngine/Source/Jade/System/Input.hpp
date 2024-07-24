#pragma once

#include "./../Core.hpp"

namespace Jade {
	
	enum struct KeyCode : uint8_t {
		None = 0,
		
		Space = ' ',
		Escape, Tab, Backspace, Delete, Enter,
		
		LeftShift, RightShift,
		LeftCtrl, RightCtrl,
		LeftAlt, RightAlt,
	
		Zero = '0',
		One, Two, Three, Four, Five, Six, Seven, Eight, Nine,
		
		A = 'A',
		B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	};
	
	enum struct MouseButton : uint8_t {
		None = 0,
		Left, Middle, Right, Thumb1, Thumb2	
	};
	
	bool IsKey(KeyCode key);
	inline bool IsKey(char key) { return IsKey(((KeyCode)key)); }
	bool IsMouseButton(MouseButton button);
	uint32_t GetMouseX();
	uint32_t GetMouseY();
	
}