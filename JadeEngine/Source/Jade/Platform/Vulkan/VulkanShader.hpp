#pragma once

#include "./../../Core.hpp"
#include "./../../Graphics/InputLayout.hpp"
#include "./../NativeShader.hpp"
#include "./Vulkan.hpp"
#include "./VulkanGraphicsContext.hpp"
#include "./VulkanRenderPass.hpp"

namespace Jade {

	namespace Vulkan {
	
		class VulkanShader : public Native::NativeShader {
		public:
			VulkanShader(const Ref<VulkanGraphicsContext> &context, const uint8_t *vertexShaderBinary, size_t vertexBinarySize, const uint8_t *fragmentShaderBinary, size_t fragmentBinarySize);
			~VulkanShader();
			
			inline VkShaderModule GetVertexShaderModule() const { return m_VertexShaderModule; }
			inline VkShaderModule GetFragmentShaderModule() const { return m_FragmentShaderModule; }
			 
		private:
			Ref<VulkanGraphicsContext> m_Context;
			
			VkShaderModule m_VertexShaderModule = VK_NULL_HANDLE;
			VkShaderModule m_FragmentShaderModule = VK_NULL_HANDLE;
		};
		
		class VulkanRenderPipeline : public Native::NativeRenderPipeline {
		public:
			VulkanRenderPipeline(const Ref<VulkanGraphicsContext> &context, const Ref<VulkanRenderPass> &renderPass, const Ref<VulkanShader> &shader, const InputLayout &layout);
			~VulkanRenderPipeline();
			
			inline VkPipelineLayout GetPipelineLayout() { return m_PipelineLayout; }
			inline VkPipeline GetPipeline() { return m_Pipeline; }
			inline VkViewport GetViewport() { return m_Viewport; }
			inline VkRect2D GetScissor() { return m_Scissor; }
			
			inline Ref<VulkanRenderPass> GetRenderPass() { return m_RenderPass; }
			inline Ref<VulkanShader> GetShader() { return m_Shader; }
			
		private:
			Ref<VulkanGraphicsContext> m_Context;
			Ref<VulkanRenderPass> m_RenderPass;
			Ref<VulkanShader> m_Shader;
			
			VkViewport m_Viewport;
			VkRect2D m_Scissor;
			
			VkPipelineLayout m_PipelineLayout;
			VkPipeline m_Pipeline;
		};
	
	}

}