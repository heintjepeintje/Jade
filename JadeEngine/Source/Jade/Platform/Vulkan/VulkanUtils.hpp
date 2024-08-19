#pragma once

#include "./Vulkan.hpp"
#include "./VulkanGraphicsContext.hpp"

namespace Jade {

	namespace Vulkan {

		class VulkanUtils {
		public:
			static uint32_t FindMemoryTypeIndex(const Ref<VulkanGraphicsContext> &context, const VkMemoryRequirements &req, uint32_t properties);
			static VkCommandBuffer BeginTransferCommands(const Ref<VulkanGraphicsContext> &context);
			static void EndTransferCommands(const Ref<VulkanGraphicsContext> &context, VkCommandBuffer buffer);
		};
	
	}

}