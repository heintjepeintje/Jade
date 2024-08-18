#pragma once

#include <Jade/Core.hpp>
#include "./../Graphics/InputLayout.hpp"
#include "./NativeRenderPass.hpp"
#include "./NativeShader.hpp"
#include "./NativeBuffer.hpp"

namespace Jade {

	enum class RenderPipelineInputElementType {
		None = 0,
		UniformBuffer, Sampler	
	};

	struct RenderPipelineInputElement {
		uint32_t BindingIndex;
		RenderPipelineInputElementType ElementType;
	};

	namespace Native {
	
		class NativeRenderPipeline {
		public:
			static Ref<Native::NativeRenderPipeline> Create(const Ref<NativeGraphicsContext> &context, const Ref<NativeRenderPass> &renderPass, const Ref<NativeShader> &shader, const std::vector<RenderPipelineInputElement> &element, const InputLayout &layout);
			
		public:
			NativeRenderPipeline() = default;
			virtual ~NativeRenderPipeline() = default;

			virtual void SetInputElement(uint32_t index, const Ref<NativeUniformBuffer> &buffer) = 0;
		};
	
	}

}