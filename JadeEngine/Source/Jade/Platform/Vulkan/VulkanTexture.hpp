#pragma once

#include "./../NativeTexture.hpp"
#include "./VulkanGraphicsContext.hpp"

namespace Jade {

	namespace Vulkan {
	
		class VulkanTexture2D : public Native::NativeTexture2D {
		public:
			VulkanTexture2D(const Ref<VulkanGraphicsContext> &context, uint32_t width, uint32_t height, uint32_t bpp, void *data);
			~VulkanTexture2D();

			inline VkImage GetImage() const { return m_Image; }
			inline VkDeviceMemory GetDeviceMemory() const { return m_Memory; }
			inline VkImageView GetImageView() const { return m_ImageView; }
			inline VkSampler GetSampler() const { return m_Sampler; }

		private:
			Ref<VulkanGraphicsContext> m_Context;
			VkImage m_Image;
			VkDeviceMemory m_Memory;
			VkImageView m_ImageView;
			VkSampler m_Sampler;
		};
	
	}

}