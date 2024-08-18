#pragma once

#include "./../Core.hpp"
#include "./NativeGraphicsContext.hpp"
#include "./NativeRenderPass.hpp"

namespace Jade {

	namespace Native {
	
		class NativeShader {
		public:
			static Ref<Native::NativeShader> Create(const Ref<NativeGraphicsContext> &context, const uint8_t *vertex, size_t vertexSize, const uint8_t *fragment, size_t fragmentSize);
			
		public:
			NativeShader() = default;
			virtual ~NativeShader() = default;
		};
		
	}

}