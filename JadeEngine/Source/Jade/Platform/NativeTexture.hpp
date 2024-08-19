#pragma once

#include <Jade/Core.hpp>

#include "./NativeGraphicsContext.hpp"

namespace Jade {

	namespace Native {
	
		class NativeTexture2D {
		public:
			static Ref<NativeTexture2D> Create(const Ref<NativeGraphicsContext> &context, uint32_t width, uint32_t height, uint32_t bpp, void *data);

		public:
			NativeTexture2D() = default;
			virtual ~NativeTexture2D() = default; 
		};
	
	}

}