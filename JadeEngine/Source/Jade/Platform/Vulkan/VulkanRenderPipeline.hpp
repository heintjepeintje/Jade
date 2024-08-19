#pragma once

#include <Jade/Core.hpp>

#include "./../NativeRenderPipeline.hpp"
#include "./Vulkan.hpp"
#include "./VulkanGraphicsContext.hpp"
#include "./VulkanRenderPass.hpp"
#include "./VulkanShader.hpp"

namespace Jade {

	namespace Vulkan {
	
		class VulkanRenderPipeline : public Native::NativeRenderPipeline {
		public:
			VulkanRenderPipeline(const Ref<VulkanGraphicsContext> &context, const Ref<VulkanRenderPass> &renderPass, const Ref<VulkanShader> &shader, const std::vector<RenderPipelineInputElement> &elements, const InputLayout &layout);
			~VulkanRenderPipeline();

			virtual void SetInputElement(uint32_t index, const Ref<Native::NativeUniformBuffer> &buffer) override;
			virtual void SetInputElement(uint32_t index, const Ref<Native::NativeTexture2D> &texture) override;
			
			inline VkPipelineLayout GetPipelineLayout() { return m_PipelineLayout; }
			inline VkPipeline GetPipeline() { return m_Pipeline; }
			inline VkViewport GetViewport() { return m_Viewport; }
			inline VkRect2D GetScissor() { return m_Scissor; }

			inline VkDescriptorSet GetDescriptorPool() { return m_DescriptorSet; }
			inline VkDescriptorSet GetDescriptorSetLayout() { return m_DescriptorSet; }
			inline VkDescriptorSet GetDescriptorSet() { return m_DescriptorSet; }
			
			inline Ref<VulkanRenderPass> GetRenderPass() { return m_RenderPass; }
			inline Ref<VulkanShader> GetShader() { return m_Shader; }
			
		private:
			Ref<VulkanGraphicsContext> m_Context;
			Ref<VulkanRenderPass> m_RenderPass;
			Ref<VulkanShader> m_Shader;
			
			VkViewport m_Viewport;
			VkRect2D m_Scissor;
			
			VkDescriptorSetLayout m_DescriptorSetLayout;
			VkDescriptorPool m_DescriptorPool;
			VkDescriptorSet m_DescriptorSet;

			VkPipelineLayout m_PipelineLayout;
			VkPipeline m_Pipeline;
		};
	
	}

}