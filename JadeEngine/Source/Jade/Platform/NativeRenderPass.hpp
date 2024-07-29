#pragma once

#include "./NativeGraphicsContext.hpp"
#include "./../Core.hpp"

namespace Jade {

	namespace Native {
	
		class NativeRenderPass {
		public:
			static Ref<NativeRenderPass> Create(const Ref<NativeGraphicsContext> &context);
			
		public:
			NativeRenderPass() = default;
			virtual ~NativeRenderPass() = default;
		};
		
	}
	
}