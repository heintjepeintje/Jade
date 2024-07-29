#include "./VulkanRenderPass.hpp"

namespace Jade {

	namespace Vulkan {
	
		VulkanRenderPass::VulkanRenderPass(const Ref<VulkanGraphicsContext> &context) : m_Context(context) {			
			VkAttachmentDescription colorAttachmentDesc = {
				.flags = 0,
				.format = m_Context->GetSurfaceFormat().format,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
				.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
				.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
				.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			};
			
			VkAttachmentReference colorAttachmentReference = {
				.attachment = 0,
				.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
			};
			
			VkSubpassDescription subpassDescription = {
				.flags = 0,
				.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
				.inputAttachmentCount = 0,
				.pInputAttachments = nullptr,
				.colorAttachmentCount = 1,
				.pColorAttachments = &colorAttachmentReference,
				.pResolveAttachments = nullptr,
				.pDepthStencilAttachment = nullptr,
				.preserveAttachmentCount = 0,
				.pPreserveAttachments = nullptr
			};
			
			VkSubpassDependency subpassDependency = {
				.srcSubpass = VK_SUBPASS_EXTERNAL,
				.dstSubpass = 0,
				.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
				.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
				.srcAccessMask = 0,
				.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
				.dependencyFlags = 0
			};
			
			VkRenderPassCreateInfo renderPassInfo = {
				.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.attachmentCount = 1,
				.pAttachments = &colorAttachmentDesc,
				.subpassCount = 1,
				.pSubpasses = &subpassDescription,
				.dependencyCount = 1,
				.pDependencies = &subpassDependency
			};
			
			JD_VULKAN_CALL(vkCreateRenderPass(
				m_Context->GetLogicalDevice(),
				&renderPassInfo,
				nullptr,
				&m_RenderPass
			));
		}
		
		VulkanRenderPass::~VulkanRenderPass() {
			vkDestroyRenderPass(
				m_Context->GetLogicalDevice(),
				m_RenderPass,
				nullptr
			);
		}
	
	}
	
	namespace Native {
	
		Ref<NativeRenderPass> NativeRenderPass::Create(const Ref<NativeGraphicsContext> &context) {
			return MakeRef<Vulkan::VulkanRenderPass>(
				CastRef<Vulkan::VulkanGraphicsContext>(context)
			);
		}
	
	}

}