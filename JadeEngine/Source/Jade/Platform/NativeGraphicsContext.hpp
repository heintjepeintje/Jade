#pragma once

#include "./../Core.hpp"
#include "./NativeWindow.hpp"

namespace Jade {

	namespace Native {
	
		class NativeGraphicsContext {
		public:
			static Ref<NativeGraphicsContext> Create(const Ref<NativeWindow> &window);
			
		public:
			NativeGraphicsContext() = default;
			virtual ~NativeGraphicsContext() = default;
			
			virtual Ref<Native::NativeWindow> GetWindow() = 0;
		};
		
		class NativeSwapChain {
		public:
			static Ref<NativeSwapChain> Create(const Ref<NativeGraphicsContext> &context, uint32_t bufferCount);
			
		public:
			NativeSwapChain() = default;
			virtual ~NativeSwapChain() = default;
			
			virtual void Present() = 0;
			virtual void Resize(uint32_t width, uint32_t height) = 0;
		};
	
	}
	
}