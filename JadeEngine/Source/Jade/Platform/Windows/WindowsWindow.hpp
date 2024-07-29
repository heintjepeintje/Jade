#pragma once

#ifdef _WIN32

#include "./Windows.hpp"
#include "./../NativeWindow.hpp"

namespace Jade {

	namespace Windows {
		
		struct WindowData {
			Native::NativeWindow *Parent;
			Native::EventCallback Callback;
			uint32_t Width, Height;	
			std::string Title;
		};
	
		LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wp, LPARAM lp);
		
		class WindowsWindow : public Native::NativeWindow {
		public:
			WindowsWindow(const std::string &title, uint32_t width, uint32_t height);
			~WindowsWindow();
			
			virtual void SetVisible(bool visible) override;
			virtual void SetMaximized(bool maximized) override;
			virtual void SetEventCallback(Native::EventCallback callback) override;
			virtual void Update() override;
			
			inline virtual uint32_t GetWidth() const override { return m_Data.Width; }
			inline virtual uint32_t GetHeight() const override { return m_Data.Height; }
			inline virtual std::string GetTitle() const override { return m_Data.Title; }
			
			inline HWND GetWindowHandle() const { return m_WindowHandle; }
			
		private:
			WindowData m_Data;
			HWND m_WindowHandle;
		};
	
	}

}

#endif // _WIN32