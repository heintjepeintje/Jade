#pragma once

#include "./GraphicsContext.hpp"
#include "./../Platform/NativeShader.hpp"
#include "./../System/Memory.hpp"

namespace Jade {
	
	class Shader {
	public:
		Shader() = default;
	
		Shader(const GraphicsContext &context, const uint8_t *vertex, size_t vertexSize, const uint8_t *fragment, size_t fragmentSize) {
			m_NativeHandle = Native::NativeShader::Create(context.GetNativeHandle(), vertex, vertexSize, fragment, fragmentSize);
		}
		
		Shader(const GraphicsContext &context, const Buffer &vertexBinary, const Buffer &fragmentBinary) {
			m_NativeHandle = Native::NativeShader::Create(
				context.GetNativeHandle(),
				vertexBinary.As<uint8_t>(),
				vertexBinary.GetSize(),
				fragmentBinary.As<uint8_t>(),
				fragmentBinary.GetSize()
			);
		}
		
		~Shader() = default;
		
		inline Ref<Native::NativeShader> GetNativeHandle() { return m_NativeHandle; }
		inline const Ref<Native::NativeShader> GetNativeHandle() const { return m_NativeHandle; }
	private:
		Ref<Native::NativeShader> m_NativeHandle;
	};
	
	class RenderPipeline {
	public:
		RenderPipeline() = default;
		RenderPipeline(const GraphicsContext &context, const RenderPass &renderPass, const Shader &shader, const InputLayout &layout) {
			m_NativeHandle = Native::NativeRenderPipeline::Create(
				context.GetNativeHandle(),
				renderPass.GetNativeHandle(),
				shader.GetNativeHandle(),
				layout
			);
		}
		~RenderPipeline() = default;
		
		inline Ref<Native::NativeRenderPipeline> GetNativeHandle() const { return m_NativeHandle; }
		
	private:
		Ref<Native::NativeRenderPipeline> m_NativeHandle;
	};
	
}