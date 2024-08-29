#pragma once

#include "./Vulkan.hpp"
#include "./VulkanGraphicsContext.hpp"
#include "./VulkanRenderPass.hpp"
#include "./VulkanShader.hpp"
#include "./VulkanRenderPipeline.hpp"
#include "./VulkanBuffer.hpp"
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
		
			inline VkCommandBuffer *GetCommandBuffers() { return m_CommandBuffers; }
			
		private:
			VkCommandBuffer *m_CommandBuffers;
			VkViewport m_Viewport;
			VkExtent2D m_Scissor;
			
			Ref<VulkanRenderPipeline> m_CurrentPipeline;
			
			Ref<VulkanSwapChain> m_SwapChain;
			Ref<VulkanGraphicsContext> m_Context;
		};
	
	}

}