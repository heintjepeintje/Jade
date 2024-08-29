#pragma once

#include "./GraphicsContext.hpp"
#include "./../Platform/NativeShader.hpp"
#include "./../System/Memory.hpp"

namespace Jade {
	
	class Shader {
	public:
		static Shader Create(const GraphicsContext &context, const uint8_t *vertex, size_t vertexSize, const uint8_t *fragment, size_t fragmentSize) {
			Shader shader;
			shader.m_NativeHandle = Native::NativeShader::Create(
				context.GetNativeHandle(), 
				vertex, vertexSize, 
				fragment, fragmentSize
			);
			return shader;
		}

		static Shader Create(const GraphicsContext &context, const Buffer &vertexBinary, const Buffer &fragmentBinary) {
			Shader shader;
			shader.m_NativeHandle = Native::NativeShader::Create(
				context.GetNativeHandle(),
				vertexBinary.As<uint8_t>(),
				vertexBinary.GetSize(),
				fragmentBinary.As<uint8_t>(),
				fragmentBinary.GetSize()
			);
			return shader;
		}

	public:
		Shader() = default;
		~Shader() = default;
		
		inline Ref<Native::NativeShader> GetNativeHandle() { return m_NativeHandle; }
		inline const Ref<Native::NativeShader> GetNativeHandle() const { return m_NativeHandle; }
	private:
		Ref<Native::NativeShader> m_NativeHandle;
	};
	
}