#pragma once

#include "./../Core.hpp"
#include "./NativeGraphicsContext.hpp"
#include "./NativeRenderPass.hpp"

namespace Jade {

	namespace Native {
	
		class NativeFramebuffer {
		public:
			static Ref<NativeFramebuffer> Create(const Ref<NativeGraphicsContext> &context, const Ref<NativeSwapChain> &swapChain, const Ref<NativeRenderPass> &renderPasss);
			
		public:
			NativeFramebuffer() = default;
			virtual ~NativeFramebuffer() = default;
		};
	
	}

}