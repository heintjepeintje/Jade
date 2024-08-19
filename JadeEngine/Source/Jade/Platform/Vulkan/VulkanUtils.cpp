#include "./VulkanUtils.hpp"

namespace Jade {

	namespace Vulkan {
	
		uint32_t VulkanUtils::FindMemoryTypeIndex(const Ref<VulkanGraphicsContext> &context, const VkMemoryRequirements &req, uint32_t properties) {
			uint32_t index = 0;
			
			for (uint32_t i = 0; i < context->GetPhysicalDeviceMemoryProperties().memoryTypeCount; i++) {
				if ((req.memoryTypeBits & (1 << i)) && 
					(context->GetPhysicalDeviceMemoryProperties().memoryTypes[i].propertyFlags & properties) == properties) {
					index = i;
					break;
				}
			}

			return index;
		}
	
		VkCommandBuffer VulkanUtils::BeginTransferCommands(const Ref<VulkanGraphicsContext> &context) {
			VkCommandBufferAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandPool = context->GetGraphicsCommandPool();
			allocInfo.commandBufferCount = 1;

			VkCommandBuffer buffer;
			vkAllocateCommandBuffers(context->GetLogicalDevice(), &allocInfo, &buffer);

			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			vkBeginCommandBuffer(buffer, &beginInfo);

			return buffer;	
		}

		void VulkanUtils::EndTransferCommands(const Ref<VulkanGraphicsContext> &context, VkCommandBuffer buffer) {
			vkEndCommandBuffer(buffer);

			VkSubmitInfo submitInfo{};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &buffer;

			vkQueueSubmit(context->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
			vkQueueWaitIdle(context->GetGraphicsQueue());

			vkFreeCommandBuffers(context->GetLogicalDevice(), context->GetGraphicsCommandPool(), 1, &buffer);
		}

	}

}