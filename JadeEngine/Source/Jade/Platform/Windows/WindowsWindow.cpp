#ifdef _WIN32

#include "./WindowsWindow.hpp"
#include "./../../Event.hpp"
#include "./WindowsInput.hpp"

namespace Jade {

	namespace Windows {
	
		static void OnWindowClose(WindowData *data) {
			if (data == nullptr) return;
			
			if (!data->Callback) return;
			WindowCloseEvent event = WindowCloseEvent();
			data->Callback(event);
		}
		
		static void OnWindowResize(WindowData *data, uint16_t width, uint16_t height) {
			if (data == nullptr) return;
			data->Width = width;
			data->Height = height;
			
			if (!data->Callback) return;
			WindowResizeEvent event = WindowResizeEvent(width, height);
			data->Callback(event);
		}
		
		static void OnWindowMove(WindowData *data, uint32_t x, uint32_t y) {
			if (data == nullptr) return;
			
			if (!data->Callback) return;
			WindowMoveEvent event = WindowMoveEvent(x, y);
			data->Callback(event);
		}
		
		static void OnWindowFocus(WindowData *data, bool focus) {
			if (data == nullptr) return;
			
			if (!data->Callback) return;
			WindowFocusEvent event = WindowFocusEvent(focus);
			data->Callback(event);
		}
		
		static void OnKeyPress(WindowData *data, int key) {
			if (data == nullptr) return;
			
			
			if (!data->Callback) return;
			KeyPressEvent event = KeyPressEvent(ToJadeKey(key));
			data->Callback(event);
		}
		
		static void OnKeyRelease(WindowData *data, int key) {
			if (data == nullptr) return;
			
			
			if (!data->Callback) return;
			KeyReleaseEvent event = KeyReleaseEvent(ToJadeKey(key));
			data->Callback(event);
		}
		
		static void OnCharacterType(WindowData *data, char character) {
			if (data == nullptr) return;
			
			if (!data->Callback) return;
			CharacterTypeEvent event = CharacterTypeEvent(character);
			data->Callback(event);
		}
		
		static void OnMouseMove(WindowData *data, int16_t x, int16_t y) {
			if (data == nullptr) return;
			
			if (!data->Callback) return;
			MouseMoveEvent event = MouseMoveEvent(x, y);
			data->Callback(event);
		}
		
		static void OnMouseButtonPress(WindowData *data, int button) {
			if (data == nullptr) return;
			
			if (!data->Callback) return;
			MouseButton lmxButton = ToJadeMouseButton(button);
			MouseButtonPressEvent event = MouseButtonPressEvent(lmxButton);
			data->Callback(event);
		}
		
		static void OnMouseButtonRelease(WindowData *data, int button) {
			if (data == nullptr) return;
			
			if (!data->Callback) return;
			MouseButton lmxButton = ToJadeMouseButton(button);
			MouseButtonReleaseEvent event = MouseButtonReleaseEvent(lmxButton);
			data->Callback(event);
		}
		
		LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wp, LPARAM lp) {
			WindowData *data = (WindowData*)(void*)(uint64_t)GetWindowLongA(window, GWLP_USERDATA);
			switch (message) {
				case WM_CLOSE: OnWindowClose(data); break;
				case WM_SIZE: OnWindowResize(data, LOWORD(lp), HIWORD(lp)); break;
				case WM_MOVE: OnWindowMove(data, LOWORD(lp), HIWORD(lp)); break;
				case WM_SETFOCUS: OnWindowFocus(data, true); break;
				case WM_KILLFOCUS: OnWindowFocus(data, false); break;
				case WM_KEYDOWN: OnKeyPress(data, LOWORD(lp)); break;
				case WM_KEYUP: OnKeyRelease(data, LOWORD(lp)); break;
				case WM_CHAR: OnCharacterType(data, LOWORD(wp)); break;
				case WM_MOUSEMOVE: OnMouseMove(data, GET_X_LPARAM(lp), GET_Y_LPARAM(lp)); break;
				case WM_LBUTTONDOWN: OnMouseButtonPress(data, VK_LBUTTON); break;
				case WM_LBUTTONUP: OnMouseButtonRelease(data, VK_LBUTTON); break;
				case WM_MBUTTONDOWN: OnMouseButtonPress(data, VK_MBUTTON); break;
				case WM_MBUTTONUP: OnMouseButtonRelease(data, VK_MBUTTON); break;
				case WM_RBUTTONDOWN: OnMouseButtonPress(data, VK_RBUTTON); break;
				case WM_RBUTTONUP: OnMouseButtonRelease(data, VK_RBUTTON); break;
				case WM_XBUTTONDOWN: OnMouseButtonPress(data, HIWORD(wp) == 1 ? VK_XBUTTON1 : VK_XBUTTON2); break;
				case WM_XBUTTONUP: OnMouseButtonPress(data, HIWORD(wp) == 1 ? VK_XBUTTON1 : VK_XBUTTON2); break;
			}
			
			return DefWindowProcA(window, message, wp, lp);
		}
		
		WindowsWindow::WindowsWindow(const std::string &title, uint32_t width, uint32_t height) {
			m_Data.Callback = nullptr;
			m_Data.Parent = this;
			m_Data.Width = width;
			m_Data.Height = height;
			m_Data.Title = title;
			
			m_WindowHandle = CreateWindowExA(
				0,
				JD_WIN32_WINDOW_CLASS_NAME,
				title.c_str(),
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT,
				width, height,
				NULL,
				NULL,
				GetModuleHandle(NULL),
				NULL
			);
			
			JD_ASSERT(m_WindowHandle != NULL, "Failed to create win32 window handle");
			
			SetWindowLongA(m_WindowHandle, GWLP_USERDATA, (LONG)(uint64_t)(void*)&m_Data);
		}
		
		WindowsWindow::~WindowsWindow() {
			DestroyWindow(m_WindowHandle);
		}
		
		void WindowsWindow::SetVisible(bool visible) {
			ShowWindow(m_WindowHandle, visible ? SW_SHOW : SW_HIDE);
		}
		
		void WindowsWindow::SetMaximized(bool maximized) {
			ShowWindow(m_WindowHandle, maximized ? SW_MAXIMIZE : SW_NORMAL);
		}
		
		void WindowsWindow::SetEventCallback(Native::EventCallback callback) {
			m_Data.Callback = callback;
		}
		
		void WindowsWindow::Update() {
			MSG msg = {};
			while (PeekMessageA(&msg, m_WindowHandle, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessageA(&msg);
			}
		}
	
	}
	
	namespace Native {
	
		Ref<NativeWindow> NativeWindow::Create(const std::string &title, uint32_t width, uint32_t height) {
			return std::make_shared<Windows::WindowsWindow>(title, width, height);
		}
	
	}

}

#endif // _WIN32