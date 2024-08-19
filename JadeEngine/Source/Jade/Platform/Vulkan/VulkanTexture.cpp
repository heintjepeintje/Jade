#include "./VulkanTexture.hpp"

#include <Jade/Core.hpp>
#include "./VulkanUtils.hpp"

namespace Jade {

	namespace Vulkan {
	
		VulkanTexture2D::VulkanTexture2D(const Ref<VulkanGraphicsContext> &context, uint32_t width, uint32_t height, uint32_t bpp, void *data) : m_Context(context) {
			VkBuffer tempBuffer;
			VkDeviceMemory tempBufferMemory;

			uint32_t queueFamilyIndices[] = { m_Context->GetGraphicsQueueIndex(), m_Context->GetTransferQueueIndex() };

			VkBufferCreateInfo bufferInfo{};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.size = width * height * bpp;
			bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			bufferInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
			bufferInfo.queueFamilyIndexCount = 2;
			bufferInfo.pQueueFamilyIndices = queueFamilyIndices;

			JD_VULKAN_CALL(vkCreateBuffer(m_Context->GetLogicalDevice(), &bufferInfo, nullptr, &tempBuffer));

			VkMemoryRequirements memoryRequirements;
			vkGetBufferMemoryRequirements(m_Context->GetLogicalDevice(), tempBuffer, &memoryRequirements);

			JD_INFO("%u/%u", memoryRequirements.size, width * height * bpp);

			VkMemoryAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.memoryTypeIndex = VulkanUtils::FindMemoryTypeIndex(m_Context, memoryRequirements, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
			allocInfo.allocationSize = memoryRequirements.size;

			JD_VULKAN_CALL(vkAllocateMemory(m_Context->GetLogicalDevice(), &allocInfo, nullptr, &tempBufferMemory));

			JD_VULKAN_CALL(vkBindBufferMemory(m_Context->GetLogicalDevice(), tempBuffer, tempBufferMemory, 0));

			void *mappedBuffer;
			JD_VULKAN_CALL(vkMapMemory(m_Context->GetLogicalDevice(), tempBufferMemory, 0, VK_WHOLE_SIZE, 0, &mappedBuffer));
			std::memcpy(mappedBuffer, data, width * height * bpp);
			vkUnmapMemory(m_Context->GetLogicalDevice(), tempBufferMemory);

			VkImageCreateInfo imageInfo{};
			imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			imageInfo.imageType = VK_IMAGE_TYPE_2D;
			imageInfo.extent.width = width;
			imageInfo.extent.height = height;
			imageInfo.extent.depth = 1;
			imageInfo.mipLevels = 1;
			imageInfo.arrayLayers = 1;
			imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
			imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
			imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			imageInfo.queueFamilyIndexCount = 0;
			imageInfo.pQueueFamilyIndices = nullptr;
			imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
			
			JD_VULKAN_CALL(vkCreateImage(m_Context->GetLogicalDevice(), &imageInfo, nullptr, &m_Image));

			VkMemoryRequirements imageMemoryRequirements;
			vkGetImageMemoryRequirements(m_Context->GetLogicalDevice(), m_Image, &imageMemoryRequirements);

			VkMemoryAllocateInfo imageAllocInfo{}; 
			imageAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			imageAllocInfo.memoryTypeIndex = VulkanUtils::FindMemoryTypeIndex(m_Context, imageMemoryRequirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
			imageAllocInfo.allocationSize = imageMemoryRequirements.size;

			JD_VULKAN_CALL(vkAllocateMemory(m_Context->GetLogicalDevice(), &imageAllocInfo, nullptr, &m_Memory));

			JD_VULKAN_CALL(vkBindImageMemory(m_Context->GetLogicalDevice(), m_Image, m_Memory, 0));

			VkCommandBuffer transferCommandBuffer = VulkanUtils::BeginTransferCommands(context);

			{
				VkImageMemoryBarrier barrier{};
				barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.image = m_Image;
				barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				barrier.subresourceRange.layerCount = 1;
				barrier.subresourceRange.levelCount = 1;
				barrier.srcAccessMask = 0;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

				vkCmdPipelineBarrier(
					transferCommandBuffer, 
					VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
					VK_PIPELINE_STAGE_TRANSFER_BIT,
					0, 
					0, nullptr, 
					0, nullptr, 
					1, &barrier);
			}

			{
				VkBufferImageCopy region{};
				region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				region.imageSubresource.layerCount = 1;
				region.imageOffset = { 0, 0 };
				region.imageExtent.width = width;
				region.imageExtent.height = height;
				region.imageExtent.depth = 1;

				vkCmdCopyBufferToImage(
					transferCommandBuffer,
					tempBuffer,
					m_Image,
					VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					1,
					&region
				);
			}

			{
				VkImageMemoryBarrier barrier{};
				barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.image = m_Image;
				barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				barrier.subresourceRange.layerCount = 1;
				barrier.subresourceRange.levelCount = 1;
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

				vkCmdPipelineBarrier(
					transferCommandBuffer, 
					VK_PIPELINE_STAGE_TRANSFER_BIT,
					VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
					0, 
					0, nullptr, 
					0, nullptr, 
					1, &barrier);
				
			}

			VulkanUtils::EndTransferCommands(m_Context, transferCommandBuffer);
    		
			vkFreeMemory(m_Context->GetLogicalDevice(), tempBufferMemory, nullptr);
			vkDestroyBuffer(m_Context->GetLogicalDevice(), tempBuffer, nullptr);

			VkImageViewCreateInfo viewInfo{};
			viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewInfo.image = m_Image;
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
			viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			viewInfo.subresourceRange.levelCount = 1;
			viewInfo.subresourceRange.layerCount = 1;

			JD_VULKAN_CALL(vkCreateImageView(m_Context->GetLogicalDevice(), &viewInfo, nullptr, &m_ImageView));

			VkSamplerCreateInfo samplerInfo{};
			samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			samplerInfo.minFilter = VK_FILTER_LINEAR;
			samplerInfo.magFilter = VK_FILTER_LINEAR;
			samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.anisotropyEnable = VK_TRUE;
			samplerInfo.maxAnisotropy = m_Context->GetPhysicalDeviceProperties().limits.maxSamplerAnisotropy;
			samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
			samplerInfo.unnormalizedCoordinates = VK_FALSE;
			samplerInfo.compareEnable = VK_FALSE;
			samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
			samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			samplerInfo.mipLodBias = 0.0f;
			samplerInfo.minLod = 0.0f;
			samplerInfo.maxLod = 0.0f;

			JD_VULKAN_CALL(vkCreateSampler(m_Context->GetLogicalDevice(), &samplerInfo, nullptr, &m_Sampler));
		}

		VulkanTexture2D::~VulkanTexture2D() {
			vkDestroySampler(m_Context->GetLogicalDevice(), m_Sampler, nullptr);
			vkDestroyImageView(m_Context->GetLogicalDevice(), m_ImageView, nullptr);
			vkDestroyImage(m_Context->GetLogicalDevice(), m_Image, nullptr);
			vkFreeMemory(m_Context->GetLogicalDevice(), m_Memory, nullptr);	
		}
	
	}

	namespace Native {
	
		Ref<NativeTexture2D> NativeTexture2D::Create(const Ref<NativeGraphicsContext> &context, uint32_t width, uint32_t height, uint32_t bpp, void *data) {
			return MakeRef<Vulkan::VulkanTexture2D>(
				CastRef<Vulkan::VulkanGraphicsContext>(context),
				width, height, bpp,
				data
			);
		}
	
	}

}