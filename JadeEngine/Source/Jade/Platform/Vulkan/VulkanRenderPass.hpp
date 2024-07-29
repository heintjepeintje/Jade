#pragma once

#include "./VulkanGraphicsContext.hpp"
#include "./Vulkan.hpp"
#include "./../NativeRenderPass.hpp"

namespace Jade {

	namespace Vulkan {
	
		class VulkanRenderPass : public Native::NativeRenderPass {
		public:
			VulkanRenderPass(const Ref<VulkanGraphicsContext> &context);
			~VulkanRenderPass();
			
			inline VkRenderPass GetRenderPass() const { return m_RenderPass; }
			
		private:
			VkRenderPass m_RenderPass;
			
			Ref<VulkanGraphicsContext> m_Context;
		};
	
	}

}