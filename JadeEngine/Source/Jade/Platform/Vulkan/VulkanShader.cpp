#include "./VulkanShader.hpp"

namespace Jade {

	namespace Vulkan {
	
		VulkanShader::VulkanShader(const Ref<VulkanGraphicsContext> &context, const uint8_t *vertexShaderBinary, size_t vertexBinarySize, const uint8_t *fragmentShaderBinary, size_t fragmentBinarySize) : m_Context(context) {
			VkShaderModuleCreateInfo vertexShaderModuleInfo;
			vertexShaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			vertexShaderModuleInfo.pNext = nullptr;
			vertexShaderModuleInfo.flags = 0;
			vertexShaderModuleInfo.pCode = reinterpret_cast<const uint32_t*>(vertexShaderBinary);
			vertexShaderModuleInfo.codeSize = vertexBinarySize;
			
			JD_VULKAN_CALL(vkCreateShaderModule(m_Context->GetLogicalDevice(), &vertexShaderModuleInfo, nullptr, &m_VertexShaderModule));
		
			VkShaderModuleCreateInfo fragmentShaderModuleInfo;
			fragmentShaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			fragmentShaderModuleInfo.pNext = nullptr;
			fragmentShaderModuleInfo.flags = 0;
			fragmentShaderModuleInfo.pCode = reinterpret_cast<const uint32_t*>(fragmentShaderBinary);
			fragmentShaderModuleInfo.codeSize = fragmentBinarySize;
			
			JD_VULKAN_CALL(vkCreateShaderModule(m_Context->GetLogicalDevice(), &fragmentShaderModuleInfo, nullptr, &m_FragmentShaderModule));
		}
		
		VulkanShader::~VulkanShader() {
			vkDestroyShaderModule(m_Context->GetLogicalDevice(), m_FragmentShaderModule, nullptr);
			vkDestroyShaderModule(m_Context->GetLogicalDevice(), m_VertexShaderModule, nullptr);
		}
	
	}
	
	namespace Native {
	
		Ref<NativeShader> NativeShader::Create(const Ref<NativeGraphicsContext> &context, const uint8_t *vertexShaderBinary, size_t vertexBinarySize, const uint8_t *fragmentShaderBinary, size_t fragmentBinarySize) {
			return MakeRef<Vulkan::VulkanShader>(
				CastRef<Vulkan::VulkanGraphicsContext>(context),
				vertexShaderBinary, vertexBinarySize,
				fragmentShaderBinary, fragmentBinarySize
			);
		}
	
	}
	
}