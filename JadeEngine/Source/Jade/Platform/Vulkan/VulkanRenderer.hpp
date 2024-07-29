#pragma once

#include "./Vulkan.hpp"
#include "./VulkanGraphicsContext.hpp"
#include "./VulkanRenderPass.hpp"
#include "./VulkanShader.hpp"
#include "./VulkanMesh.hpp"
#include "./../NativeRenderer.hpp"

namespace Jade {

	namespace Vulkan {
	
		class VulkanRendererAPI : public Native::NativeRendererAPI {
		public:
			VulkanRendererAPI(const Ref<VulkanGraphicsContext> &context, const Ref<VulkanSwapChain> &swapChain);
			~VulkanRendererAPI();
			
			virtual void Begin() override;
			virtual void Clear(float r, float g, float b, float a) override;
			virtual void BeginRender(const Ref<Native::NativeRenderPipeline> &pipeline) override;
			virtual void DrawIndexed(const Ref<Native::NativeVertexBuffer> &vbo, const Ref<Native::NativeIndexBuffer> &ibo);
			virtual void EndRender() override;
			virtual void End() override;
			
			
			inline VkQueue GetGraphicsQueue() { return m_Queue; }
			inline VkCommandPool GetCommandPool() { return m_CommandPool; }
			inline VkCommandBuffer *GetCommandBuffers() { return m_CommandBuffers; }
			
		private:
			VkQueue m_Queue = VK_NULL_HANDLE;
			
			VkCommandPool m_CommandPool = VK_NULL_HANDLE;
			VkCommandBuffer *m_CommandBuffers;
			VkViewport m_Viewport;
			VkExtent2D m_Scissor;
			
			VkPipelineLayout m_PipelineLayout;
			VkPipeline m_Pipeline;
			VkRenderPass m_RenderPass;
			
			Ref<VulkanSwapChain> m_SwapChain;
			Ref<VulkanGraphicsContext> m_Context;
		};
	
	}

}