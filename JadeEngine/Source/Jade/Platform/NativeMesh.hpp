#pragma once

#include "./../Core.hpp"
#include "./NativeGraphicsContext.hpp"
#include "./../Graphics/InputLayout.hpp"

namespace Jade {

	namespace Native {
	
		class NativeVertexBuffer {
		public:
			static Ref<NativeVertexBuffer> Create(const Ref<NativeGraphicsContext> &context, void *data, size_t size);
			
		public:
			NativeVertexBuffer() = default;
			virtual ~NativeVertexBuffer() = default;
			
			virtual size_t GetSize() const = 0;
		};
		
		class NativeIndexBuffer {
		public:
			static Ref<NativeIndexBuffer> Create(const Ref<NativeGraphicsContext> &context, uint32_t *data, size_t size);
			
			virtual size_t GetSize() const = 0;
			
		public:
			NativeIndexBuffer() = default;
			virtual ~NativeIndexBuffer() = default;
		};
	
	}

}