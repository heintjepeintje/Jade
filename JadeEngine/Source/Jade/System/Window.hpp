#pragma once

#include "./../Core.hpp"
#include "./../Platform/NativeWindow.hpp"

namespace Jade {
	
	class Window {
	public:
		Window() = default;
		Window(const std::string &title, uint32_t width, uint32_t height) {
			m_NativeHandle = Native::NativeWindow::Create(title, width, height);
		}
		~Window() = default;
		
		inline void SetVisible(bool visible) { m_NativeHandle->SetVisible(visible); }
		inline void SetMaximized(bool maximized) { m_NativeHandle->SetMaximized(maximized); }
		inline void Update() { m_NativeHandle->Update(); }
		
		inline void SetEventCallback(Native::EventCallback callback) { m_NativeHandle->SetEventCallback(callback); }
		inline uint32_t GetWidth() const { return m_NativeHandle->GetWidth(); }
		inline uint32_t GetHeight() const { return m_NativeHandle->GetHeight(); }
		
	private:
		std::shared_ptr<Native::NativeWindow> m_NativeHandle;
	};

}