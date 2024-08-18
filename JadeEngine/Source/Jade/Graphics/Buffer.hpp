#pragma once

#include "./../Core.hpp"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "./../Platform/NativeBuffer.hpp"
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
		static VertexBuffer Create(const GraphicsContext &context, void *data, size_t size) {
			VertexBuffer buffer;
			buffer.m_NativeHandle = Native::NativeVertexBuffer::Create(
				context.GetNativeHandle(),
				(void*)data,
				size
			);
			return buffer;
		}
	public:
		VertexBuffer() = default;
		~VertexBuffer() = default;
		
		inline Ref<Native::NativeVertexBuffer> GetNativeHandle() const { return m_NativeHandle; }
		
	private:
		Ref<Native::NativeVertexBuffer> m_NativeHandle;
	};
	
	class IndexBuffer {
	public:
		static IndexBuffer Create(const GraphicsContext &context, uint32_t *data, size_t size) {
			IndexBuffer buffer;
			buffer.m_NativeHandle = Native::NativeIndexBuffer::Create(
				context.GetNativeHandle(),
				data,
				size
			);
			return buffer;
		}

	public:
		IndexBuffer() = default;
		~IndexBuffer() = default;
		
		inline Ref<Native::NativeIndexBuffer> GetNativeHandle() const { return m_NativeHandle; }
		
	private:
		Ref<Native::NativeIndexBuffer> m_NativeHandle;
	};

	class UniformBuffer {
	public:
		static UniformBuffer Create(const GraphicsContext &context, void *data, size_t size) {
			UniformBuffer buffer;
			buffer.m_NativeHandle = Native::NativeUniformBuffer::Create(
				context.GetNativeHandle(),
				data,
				size
			);
			return buffer;	
		}

	public:
		UniformBuffer() = default;
		~UniformBuffer() = default;

		inline Ref<Native::NativeUniformBuffer> GetNativeHandle() const { return m_NativeHandle; }
		
	private:
		std::shared_ptr<Native::NativeUniformBuffer> m_NativeHandle;
	};

}