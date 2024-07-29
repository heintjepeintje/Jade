#pragma once

#include "./../NativeMesh.hpp"
#include "./VulkanGraphicsContext.hpp"
#include "./Vulkan.hpp"

namespace Jade {

	namespace Vulkan {
	
		class VulkanVertexBuffer : public Native::NativeVertexBuffer {
		public:
			VulkanVertexBuffer(const Ref<VulkanGraphicsContext> &context, void *data, size_t size);
			~VulkanVertexBuffer();
			
			inline virtual size_t GetSize() const { return m_Size; }
		
			inline VkBuffer GetBuffer() const { return m_Buffer; }
			inline VkDeviceMemory GetMemory() const { return m_Memory; }
		
		private:
			Ref<VulkanGraphicsContext> m_Context;
			
			VkBuffer m_Buffer = VK_NULL_HANDLE;
			VkDeviceMemory m_Memory = VK_NULL_HANDLE;
			
			size_t m_Size = 0;
		};
		
		class VulkanIndexBuffer : public Native::NativeIndexBuffer {
		public:
			VulkanIndexBuffer(const Ref<VulkanGraphicsContext> &context, uint32_t *data, size_t size);
			~VulkanIndexBuffer();
			
			inline VkBuffer GetBuffer() { return m_Buffer; }
			inline VkDeviceMemory GetDeviceMemory() { return m_Memory; }
			inline virtual size_t GetSize() const { return m_Size; } 
		
		private:
			Ref<VulkanGraphicsContext> m_Context;
			
			VkBuffer m_Buffer = VK_NULL_HANDLE;
			VkDeviceMemory m_Memory = VK_NULL_HANDLE;
			
			size_t m_Size = 0;
		};
	
	}

}