#pragma once

#include "./../Core.hpp"
#include "./../Platform/NativeWindow.hpp"

namespace Jade {
	
	class Window {
	public:
		static Window Create(const std::string &title, uint32_t width, uint32_t height) {
			Window window;
			window.m_NativeHandle = Native::NativeWindow::Create(
				title, 
				width, height
			);
			return window;
		}
	public:
		Window() = default;
		~Window() = default;
		
		inline void SetVisible(bool visible) { m_NativeHandle->SetVisible(visible); }
		inline void SetMaximized(bool maximized) { m_NativeHandle->SetMaximized(maximized); }
		inline void Update() { m_NativeHandle->Update(); }
		
		inline void SetEventCallback(Native::EventCallback callback) { m_NativeHandle->SetEventCallback(callback); }
		inline uint32_t GetWidth() const { return m_NativeHandle->GetWidth(); }
		inline uint32_t GetHeight() const { return m_NativeHandle->GetHeight(); }
		
		inline const Ref<Native::NativeWindow> GetNativeHandle() const { return m_NativeHandle; }
		inline Ref<Native::NativeWindow> GetNativeHandle() { return m_NativeHandle; }
		
	private:
		Ref<Native::NativeWindow> m_NativeHandle;
	};

}