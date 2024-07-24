#if defined(_WIN32)

#include "./WindowsInput.hpp"
#include "./Windows.hpp"

namespace Jade {
	
	namespace Windows {
		
		int ToWindowsKey(KeyCode key) {
			switch (key) {
				case KeyCode::Space: return VK_SPACE;
				case KeyCode::Escape: return VK_ESCAPE;
				case KeyCode::Tab: return VK_TAB;
				case KeyCode::Backspace: return VK_BACK;
				case KeyCode::Delete: return VK_DELETE;
				case KeyCode::Enter: return VK_RETURN;
				
				case KeyCode::LeftShift: return VK_LSHIFT;
				case KeyCode::RightShift: return VK_RSHIFT;
				case KeyCode::LeftCtrl: return VK_LCONTROL;
				case KeyCode::RightCtrl: return VK_RCONTROL;
				case KeyCode::LeftAlt: return VK_LMENU;
				case KeyCode::RightAlt: return VK_RMENU;
				
				default: break;
			}
			
			if (key >= KeyCode::Zero && key <= KeyCode::Nine) {
				return ((uint16_t)key) - ((uint16_t)KeyCode::Zero) + 0x30;
			}
			
			if (key >= KeyCode::A && key <= KeyCode::Z) {
				return ((uint16_t)key);
			}
			
			if (key >= KeyCode::F1 && key <= KeyCode::F12) {
				return ((uint16_t)key) - ((uint16_t)KeyCode::F1) + VK_F1;
			}
			
			return -1;
		}
		
		KeyCode ToJadeKey(int key) {
			switch (key) {
				case VK_SPACE: return KeyCode::Space;
				case VK_ESCAPE: return KeyCode::Escape;
				case VK_TAB: return KeyCode::Tab;
				case VK_BACK: return KeyCode::Backspace;
				case VK_DELETE: return KeyCode::Delete;
				case VK_RETURN: return KeyCode::Enter;
				case VK_LSHIFT: return KeyCode::LeftShift;
				case VK_RSHIFT: return KeyCode::RightShift;
				case VK_LCONTROL: return KeyCode::LeftCtrl;
				case VK_RCONTROL: return KeyCode::RightCtrl;
				case VK_LMENU: return KeyCode::LeftAlt;
				case VK_RMENU: return KeyCode::RightAlt;
			};
			
			if (key >= 0x30 && key <= 0x39) {
				return (KeyCode)(key - 0x30 + ((uint16_t)KeyCode::Zero));
			}
			
			if (key >= 'A' && key <= 'Z') {
				return ((KeyCode)key);
			}
			
			if (key >= VK_F1 && key <= VK_F12) {
				return (KeyCode)(key - VK_F1 + ((uint16_t)KeyCode::F1));
			}
			
			return KeyCode::None;
		}
		
		
		int ToWindowsMouseButton(MouseButton button) {
			switch (button) {
				case MouseButton::Left: return VK_LBUTTON; 
				case MouseButton::Middle: return VK_MBUTTON;
				case MouseButton::Right: return VK_RBUTTON;
				case MouseButton::Thumb1: return VK_XBUTTON1;
				case MouseButton::Thumb2: return VK_XBUTTON2;
				default: return -1;
			}
		}
		
		MouseButton ToJadeMouseButton(int button) {
			switch (button) {
				case VK_LBUTTON: return MouseButton::Left; 
				case VK_MBUTTON: return MouseButton::Middle;
				case VK_RBUTTON: return MouseButton::Right;
				case VK_XBUTTON1: return MouseButton::Thumb1;
				case VK_XBUTTON2: return MouseButton::Thumb2;
				default: return MouseButton::None;
			}
		}
		
	}
	
	bool IsKey(KeyCode key) {
		SHORT state = GetKeyState(Windows::ToWindowsKey(key));
		return HIBYTE(state);
	}
	
	bool IsMouseButton(MouseButton button) {
		SHORT state = GetKeyState(Windows::ToWindowsMouseButton(button));
		return HIBYTE(state);
	}
	
	uint32_t GetMouseX() {
		POINT position;
		GetCursorPos(&position);
		return position.x;
	}
	
	uint32_t GetMouseY() {
		POINT position;
		GetCursorPos(&position);
		return position.y;
	}
	
}

#endif