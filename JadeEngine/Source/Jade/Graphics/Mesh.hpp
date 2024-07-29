#pragma once

#include "./../Core.hpp"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "./../Platform/NativeMesh.hpp"
#include "./GraphicsContext.hpp"
#include "./InputLayout.hpp"

namespace Jade {
	
	struct Vertex {
		glm::vec3 Position;
		glm::vec2 UV;
		glm::vec4 Color;
	};

	class VertexBuffer {
	public:
		VertexBuffer() = default;
		VertexBuffer(const GraphicsContext &context, void *data, size_t size) {
			m_NativeHandle = Native::NativeVertexBuffer::Create(
				context.GetNativeHandle(),
				(void*)data,
				size
			);
		}
		~VertexBuffer() = default;
		
		inline Ref<Native::NativeVertexBuffer> GetNativeHandle() const { return m_NativeHandle; }
		
	private:
		Ref<Native::NativeVertexBuffer> m_NativeHandle;
	};
	
	class IndexBuffer {
	public:
		IndexBuffer() = default;
		IndexBuffer(const GraphicsContext &context, uint32_t *data, size_t size) {
			m_NativeHandle = Native::NativeIndexBuffer::Create(
				context.GetNativeHandle(),
				data,
				size
			);
		}
		~IndexBuffer() = default;
		
		inline Ref<Native::NativeIndexBuffer> GetNativeHandle() const { return m_NativeHandle; }
		
	private:
		Ref<Native::NativeIndexBuffer> m_NativeHandle;
	};

}