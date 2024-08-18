#pragma once

#include "./../../Core.hpp"
#include "./../../Graphics/InputLayout.hpp"
#include "./../NativeShader.hpp"
#include "./Vulkan.hpp"
#include "./VulkanGraphicsContext.hpp"
#include "./VulkanRenderPass.hpp"

namespace Jade {

	namespace Vulkan {
	
		class VulkanShader : public Native::NativeShader {
		public:
			VulkanShader(const Ref<VulkanGraphicsContext> &context, const uint8_t *vertexShaderBinary, size_t vertexBinarySize, const uint8_t *fragmentShaderBinary, size_t fragmentBinarySize);
			~VulkanShader();
			
			inline VkShaderModule GetVertexShaderModule() const { return m_VertexShaderModule; }
			inline VkShaderModule GetFragmentShaderModule() const { return m_FragmentShaderModule; }
			 
		private:
			Ref<VulkanGraphicsContext> m_Context;
			
			VkShaderModule m_VertexShaderModule = VK_NULL_HANDLE;
			VkShaderModule m_FragmentShaderModule = VK_NULL_HANDLE;
		};
	
	}

}