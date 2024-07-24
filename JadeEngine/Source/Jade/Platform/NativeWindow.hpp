#pragma once

#include "./../Event.hpp"

namespace Jade {

	namespace Native {
	
		typedef void(*EventCallback)(const Event&);
	
		class NativeWindow {
		public:
			static std::shared_ptr<NativeWindow> Create(const std::string &title, uint32_t width, uint32_t height);
			
		public:
			NativeWindow() = default;
			virtual ~NativeWindow() = default;
			
			virtual void SetVisible(bool visible) = 0;
			virtual void SetMaximized(bool maximized) = 0;
			virtual void SetEventCallback(EventCallback callback) = 0;
			virtual void Update() = 0;
			
			virtual uint32_t GetWidth() const = 0;
			virtual uint32_t GetHeight() const = 0;
		};
	
	}

}