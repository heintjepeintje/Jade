#pragma once

#include "./GraphicsContext.hpp"
#include "./RenderPass.hpp"
#include "./Shader.hpp"
#include "./InputLayout.hpp"
#include "./Texture.hpp"
#include "./../Platform/NativeRenderPipeline.hpp"

namespace Jade {

	class RenderPipeline {
	public:
		RenderPipeline() = default;
		RenderPipeline(const GraphicsContext &context, const RenderPass &renderPass, const Shader &shader, const std::vector<RenderPipelineInputElement> &elements, const InputLayout &layout) {
			m_NativeHandle = Native::NativeRenderPipeline::Create(
				context.GetNativeHandle(),
				renderPass.GetNativeHandle(),
				shader.GetNativeHandle(),
				elements,
				layout
			);
		}
		~RenderPipeline() = default;

		inline void SetInputElement(uint32_t index, const UniformBuffer &buffer) { return m_NativeHandle->SetInputElement(index, buffer.GetNativeHandle()); }
		inline void SetInputElement(uint32_t index, const Texture2D &texture) { return m_NativeHandle->SetInputElement(index, texture.GetNativeHandle()); }

		inline Ref<Native::NativeRenderPipeline> GetNativeHandle() const { return m_NativeHandle; }
		
	private:
		Ref<Native::NativeRenderPipeline> m_NativeHandle;
	};

}