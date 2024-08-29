#include "./../../Core.hpp"
#include "./VulkanBuffer.hpp"
#include "./../../System/Memory.hpp"

namespace Jade {

	namespace Vulkan {
	
		VulkanVertexBuffer::VulkanVertexBuffer(const Ref<VulkanGraphicsContext> &context, void *data, size_t size) : m_Context(context), m_Size(size) {
			// TODO: Optimize memory type by using staging buffers
			
			VkBufferCreateInfo bufferInfo = {
				.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.size = size,
				.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
				.queueFamilyIndexCount = 0,
				.pQueueFamilyIndices = nullptr
			};
			
			JD_VULKAN_CALL(vkCreateBuffer(
				m_Context->GetLogicalDevice(),
				&bufferInfo,
				nullptr,
				&m_Buffer
			));
			
			VkMemoryRequirements memoryRequirements;
			vkGetBufferMemoryRequirements(
				m_Context->GetLogicalDevice(),
				m_Buffer,
				&memoryRequirements
			);
			
			VkPhysicalDeviceMemoryProperties memoryProperties;
			vkGetPhysicalDeviceMemoryProperties(
				m_Context->GetPhysicalDevice(), 
				&memoryProperties
			);
			
			uint32_t properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			uint32_t index = 0;
			
			for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
				if ((memoryRequirements.memoryTypeBits & (1 << i)) && 
					(memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
					index = i;
					break;
				}
			}
			
			VkMemoryAllocateInfo allocateInfo = {
				.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				.pNext = nullptr,
				.allocationSize = memoryRequirements.size,
				.memoryTypeIndex = index,
			};
			
			JD_VULKAN_CALL(vkAllocateMemory(
				m_Context->GetLogicalDevice(),
				&allocateInfo,
				nullptr,
				&m_Memory
			));
			
			JD_VULKAN_CALL(vkBindBufferMemory(
				m_Context->GetLogicalDevice(),
				m_Buffer,
				m_Memory,
				0
			));
			
			void *buffer;
			JD_VULKAN_CALL(vkMapMemory(
				m_Context->GetLogicalDevice(),
				m_Memory,
				0,
				bufferInfo.size,
				0,
				&buffer
			));
			
			std::memcpy(buffer, data, size);
			
			vkUnmapMemory(
				m_Context->GetLogicalDevice(),
				m_Memory
			);
		}
		
		VulkanVertexBuffer::~VulkanVertexBuffer() {
			vkFreeMemory(
				m_Context->GetLogicalDevice(),
				m_Memory,
				nullptr
			);

			vkDestroyBuffer(
				m_Context->GetLogicalDevice(),
				m_Buffer,
				nullptr
			);
		}
		
		VulkanIndexBuffer::VulkanIndexBuffer(const Ref<VulkanGraphicsContext> &context, uint32_t *data, size_t size) : m_Context(context), m_Size(size) {
			
			// TODO: Optimize memory type by using staging buffers
			VkBufferCreateInfo bufferInfo = {
				.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.size = size,
				.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
				.queueFamilyIndexCount = 0,
				.pQueueFamilyIndices = nullptr
			};
			
			JD_VULKAN_CALL(vkCreateBuffer(
				m_Context->GetLogicalDevice(),
				&bufferInfo,
				nullptr,
				&m_Buffer
			));
			
			VkMemoryRequirements memoryRequirements;
			vkGetBufferMemoryRequirements(
				m_Context->GetLogicalDevice(),
				m_Buffer,
				&memoryRequirements
			);
			
			VkPhysicalDeviceMemoryProperties memoryProperties;
			vkGetPhysicalDeviceMemoryProperties(
				m_Context->GetPhysicalDevice(), 
				&memoryProperties
			);
			
			uint32_t properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			uint32_t index = 0;
			
			for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
				if ((memoryRequirements.memoryTypeBits & (1 << i)) && 
					(memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
					index = i;
					break;
				}
			}
			
			VkMemoryAllocateInfo allocateInfo = {
				.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				.pNext = nullptr,
				.allocationSize = memoryRequirements.size,
				.memoryTypeIndex = index,
			};
			
			JD_VULKAN_CALL(vkAllocateMemory(
				m_Context->GetLogicalDevice(),
				&allocateInfo,
				nullptr,
				&m_Memory
			));
			
			JD_VULKAN_CALL(vkBindBufferMemory(
				m_Context->GetLogicalDevice(),
				m_Buffer,
				m_Memory,
				0
			));
			
			void *buffer;
			JD_VULKAN_CALL(vkMapMemory(
				m_Context->GetLogicalDevice(),
				m_Memory,
				0,
				bufferInfo.size,
				0,
				&buffer
			));
			
			std::memcpy(buffer, data, size);
			
			vkUnmapMemory(
				m_Context->GetLogicalDevice(),
				m_Memory
			);
		}
		
		VulkanIndexBuffer::~VulkanIndexBuffer() {
			vkFreeMemory(
				m_Context->GetLogicalDevice(),
				m_Memory,
				nullptr
			);

			vkDestroyBuffer(
				m_Context->GetLogicalDevice(),
				m_Buffer,
				nullptr
			);
		}

		VulkanUniformBuffer::VulkanUniformBuffer(const Ref<VulkanGraphicsContext> &context, size_t size) : m_Context(context), m_Size(size) {
			VkBufferCreateInfo bufferInfo = {
				.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.size = size,
				.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
				.queueFamilyIndexCount = 0,
				.pQueueFamilyIndices = nullptr
			};
			
			JD_VULKAN_CALL(vkCreateBuffer(
				m_Context->GetLogicalDevice(),
				&bufferInfo,
				nullptr,
				&m_Buffer
			));
			
			VkMemoryRequirements memoryRequirements;
			vkGetBufferMemoryRequirements(
				m_Context->GetLogicalDevice(),
				m_Buffer,
				&memoryRequirements
			);
			
			VkPhysicalDeviceMemoryProperties memoryProperties;
			vkGetPhysicalDeviceMemoryProperties(
				m_Context->GetPhysicalDevice(), 
				&memoryProperties
			);
			
			uint32_t properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			uint32_t index = 0;
			
			for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
				if ((memoryRequirements.memoryTypeBits & (1 << i)) && 
					(memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
					index = i;
					break;
				}
			}
			
			VkMemoryAllocateInfo allocateInfo = {
				.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				.pNext = nullptr,
				.allocationSize = memoryRequirements.size,
				.memoryTypeIndex = index,
			};
			
			JD_VULKAN_CALL(vkAllocateMemory(
				m_Context->GetLogicalDevice(),
				&allocateInfo,
				nullptr,
				&m_Memory
			));
			
			JD_VULKAN_CALL(vkBindBufferMemory(
				m_Context->GetLogicalDevice(),
				m_Buffer,
				m_Memory,
				0
			));
		}

		VulkanUniformBuffer::~VulkanUniformBuffer() {
			vkFreeMemory(
				m_Context->GetLogicalDevice(),
				m_Memory,
				nullptr
			);

			vkDestroyBuffer(
				m_Context->GetLogicalDevice(),
				m_Buffer,
				nullptr
			);
		}

		void *VulkanUniformBuffer::Map() const {
			void *map;
			JD_VULKAN_CALL(vkMapMemory(m_Context->GetLogicalDevice(), m_Memory, 0, VK_WHOLE_SIZE, 0, &map));
			return map;
		}

		void VulkanUniformBuffer::Unmap() {
			vkUnmapMemory(m_Context->GetLogicalDevice(), m_Memory);
		}
	
	}
	
	namespace Native {
	
		Ref<NativeVertexBuffer> NativeVertexBuffer::Create(const Ref<NativeGraphicsContext> &context, void *data, size_t size) {
			return std::make_shared<Vulkan::VulkanVertexBuffer>(
				CastRef<Vulkan::VulkanGraphicsContext>(context),
				data, size
			);
		}
		
		Ref<NativeIndexBuffer> NativeIndexBuffer::Create(const Ref<NativeGraphicsContext> &context, uint32_t *data, size_t size) {
			return std::make_shared<Vulkan::VulkanIndexBuffer>(
				CastRef<Vulkan::VulkanGraphicsContext>(context),
				data, size
			);
		}

		Ref<NativeUniformBuffer> NativeUniformBuffer::Create(const Ref<NativeGraphicsContext> &context, size_t size) {
			return std::make_shared<Vulkan::VulkanUniformBuffer>(
				CastRef<Vulkan::VulkanGraphicsContext>(context),
				size
			);
		}
	
	}

}