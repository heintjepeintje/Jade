#include "./VulkanRenderer.hpp"

namespace Jade {

	namespace Vulkan {
	
		VulkanRendererAPI::VulkanRendererAPI(const Ref<VulkanGraphicsContext> &context, const Ref<VulkanSwapChain> &swapChain) : m_SwapChain(swapChain), m_Context(context) {	
			vkGetDeviceQueue(m_Context->GetLogicalDevice(), m_Context->GetGraphicsQueueIndex(), 0, &m_Queue);
			
			VkCommandPoolCreateInfo commandPoolInfo = {
				.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
				.pNext = nullptr,
				.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
				.queueFamilyIndex = m_Context->GetGraphicsQueueIndex()
			};
			
			JD_VULKAN_CALL(vkCreateCommandPool(
				m_Context->GetLogicalDevice(),
				&commandPoolInfo, 
				nullptr, 
				&m_CommandPool
			));
			
			m_CommandBuffers = Alloc<VkCommandBuffer>(m_SwapChain->GetImageCount());
			
			VkCommandBufferAllocateInfo commandBufferInfo = {
				.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
				.pNext = nullptr,
				.commandPool = m_CommandPool,
				.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
				.commandBufferCount = m_SwapChain->GetImageCount()
			};
			
			JD_VULKAN_CALL(vkAllocateCommandBuffers(
				m_Context->GetLogicalDevice(), 
				&commandBufferInfo, 
				m_CommandBuffers
			));
		}
		
		VulkanRendererAPI::~VulkanRendererAPI() {
			JD_VULKAN_CALL(vkQueueWaitIdle(m_Queue));
			
			// vkDestroyPipelineLayout(
			// 	m_Context->GetLogicalDevice(),
			// 	m_PipelineLayout,
			// 	nullptr
			// );
			
			vkFreeCommandBuffers(
				m_Context->GetLogicalDevice(), 
				m_CommandPool, 
				m_SwapChain->GetImageCount(), 
				m_CommandBuffers
			);
			
			Free(m_CommandBuffers);
			
			vkDestroyCommandPool(
				m_Context->GetLogicalDevice(), 
				m_CommandPool, 
				nullptr
			);
		}
		
		void VulkanRendererAPI::Begin() {
			VkCommandBufferBeginInfo beginInfo = {
				.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
				.pNext = nullptr,
				.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
				.pInheritanceInfo = nullptr
			};
			
			JD_VULKAN_CALL(vkBeginCommandBuffer(
				m_CommandBuffers[m_SwapChain->GetNextImageIndex()], 
				&beginInfo
			));
			
			VkImageSubresourceRange imageRange = {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			}; 
			
			VkImageMemoryBarrier presentToClearBarrier = {
				.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
				.pNext = nullptr,
				.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT,
				.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
				.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
				.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.image = m_SwapChain->GetImage(m_SwapChain->GetNextImageIndex()),
				.subresourceRange = imageRange
			};
			
			vkCmdPipelineBarrier(
				m_CommandBuffers[m_SwapChain->GetNextImageIndex()],
				VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				0,
				0, nullptr,
				0, nullptr,
				1, &presentToClearBarrier
			); 
		}
		
		void VulkanRendererAPI::BeginRender(const Ref<Native::NativeRenderPipeline> &pipeline) {
			Ref<VulkanRenderPipeline> vulkanRenderPipeline = CastRef<VulkanRenderPipeline>(pipeline);
			
			VkClearValue clearValue = {
				.color = {
					.float32 = {
						1.0f,
						1.0f,
						1.0f,
						1.0f,
					}
				}
			};
			
			VkRenderPassBeginInfo renderPassBeginInfo = {
				.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
				.pNext = nullptr,
				.renderPass = vulkanRenderPipeline->GetRenderPass()->GetRenderPass(),
				.framebuffer = m_SwapChain->GetFramebuffer(m_SwapChain->GetNextImageIndex()),
				.renderArea = {
					.offset = {
						.x = 0,
						.y = 0,
					},
					.extent = {
						.width = m_Context->GetWindow()->GetWidth(),
						.height = m_Context->GetWindow()->GetHeight()
					}
				},
				.clearValueCount = 1,
				.pClearValues = &clearValue,
			};
			
			vkCmdBindPipeline(
				m_CommandBuffers[m_SwapChain->GetNextImageIndex()],
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				vulkanRenderPipeline->GetPipeline()
			);
			
			vkCmdBeginRenderPass(
				m_CommandBuffers[m_SwapChain->GetNextImageIndex()],
				&renderPassBeginInfo,
				VK_SUBPASS_CONTENTS_INLINE
			);
			
			VkViewport viewports[] = { vulkanRenderPipeline->GetViewport() };
			VkRect2D scissors[] = { vulkanRenderPipeline->GetScissor() };
			
			vkCmdSetViewport(
				m_CommandBuffers[m_SwapChain->GetNextImageIndex()],
				0,
				JD_ARRAYSIZE(viewports),
				viewports);
				
			vkCmdSetScissor(
				m_CommandBuffers[m_SwapChain->GetNextImageIndex()],
				0,
				JD_ARRAYSIZE(scissors),
				scissors
			);

			VkDescriptorSet descriptorSets[1] = { vulkanRenderPipeline->GetDescriptorSet() };

			vkCmdBindDescriptorSets(
				m_CommandBuffers[m_SwapChain->GetNextImageIndex()], 
				VK_PIPELINE_BIND_POINT_GRAPHICS, 
				vulkanRenderPipeline->GetPipelineLayout(),
				0, 1,
				descriptorSets,
				0,
				nullptr
			);
		}
		
		void VulkanRendererAPI::Clear(float r, float g, float b, float a) {
			VkClearColorValue clearColor = { r, g, b, a };
			
			VkImageSubresourceRange imageRange = {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			}; 
			
			vkCmdClearColorImage(
				m_CommandBuffers[m_SwapChain->GetNextImageIndex()], 
				m_SwapChain->GetImage(m_SwapChain->GetNextImageIndex()), 
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
				&clearColor, 
				1, 
				&imageRange
			);
			
		}
		
		void VulkanRendererAPI::DrawIndexed(const Ref<Native::NativeVertexBuffer> &vbo, const Ref<Native::NativeIndexBuffer> &ibo) {
			Ref<VulkanVertexBuffer> vulkanVertexBuffer = CastRef<VulkanVertexBuffer>(vbo);
			Ref<VulkanIndexBuffer> vulkanIndexBuffer = CastRef<VulkanIndexBuffer>(ibo);
			
			VkBuffer vertexBuffers[] = { vulkanVertexBuffer->GetBuffer() };
			VkDeviceSize offsets[] = { 0 };
			
			vkCmdBindVertexBuffers(
				m_CommandBuffers[m_SwapChain->GetNextImageIndex()],
				0,
				1,
				vertexBuffers,
				offsets
			);
			
			vkCmdBindIndexBuffer(
				m_CommandBuffers[m_SwapChain->GetNextImageIndex()],
				vulkanIndexBuffer->GetBuffer(),
				0,
				VK_INDEX_TYPE_UINT32
			);
				
			
			vkCmdDrawIndexed(
				m_CommandBuffers[m_SwapChain->GetNextImageIndex()],
				vulkanIndexBuffer->GetSize() / sizeof(uint32_t),
				1, 0, 0, 0
			); 
			
		}
		
		void VulkanRendererAPI::EndRender() {
			vkCmdEndRenderPass(
				m_CommandBuffers[m_SwapChain->GetNextImageIndex()]
			);
		}
		
		void VulkanRendererAPI::End() {			
			VkImageSubresourceRange imageRange = {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			}; 
			
			VkImageMemoryBarrier clearToPresentBarrier = {
				.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
				.pNext = nullptr,
				.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
				.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT,
				.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
				.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.image = m_SwapChain->GetImage(m_SwapChain->GetNextImageIndex()),
				.subresourceRange = imageRange
			};
			
			vkCmdPipelineBarrier(
				m_CommandBuffers[m_SwapChain->GetNextImageIndex()],
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
				0,
				0, nullptr,
				0, nullptr,
				1, &clearToPresentBarrier
			);
			
			JD_VULKAN_CALL(vkEndCommandBuffer(m_CommandBuffers[m_SwapChain->GetNextImageIndex()]));
			
			VkFence waitFences[] = { m_SwapChain->GetPresentCompleteFence() };
			
			JD_VULKAN_CALL(vkWaitForFences(
				m_Context->GetLogicalDevice(),
				JD_ARRAYSIZE(waitFences),
				waitFences,
				VK_TRUE,
				UINT64_MAX
			));
			
			JD_VULKAN_CALL(vkResetFences(
				m_Context->GetLogicalDevice(),
				JD_ARRAYSIZE(waitFences),
				waitFences
			));
			
			VkPipelineStageFlags waitFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			
			VkSemaphore waitSemaphores[] = { m_SwapChain->GetPresentCompleteSemaphore() };
			VkSemaphore signalSemaphores[] = { m_SwapChain->GetRenderCompleteSemaphore() };
			
			VkSubmitInfo submitInfo = {
				.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
				.pNext = nullptr,
				.waitSemaphoreCount = JD_ARRAYSIZE(waitSemaphores),
				.pWaitSemaphores = waitSemaphores,
				.pWaitDstStageMask = &waitFlags,
				.commandBufferCount = 1,
				.pCommandBuffers = &m_CommandBuffers[m_SwapChain->GetNextImageIndex()],
				.signalSemaphoreCount = 1,
				.pSignalSemaphores = signalSemaphores
			};
			
			JD_VULKAN_CALL(vkQueueSubmit(m_Queue, 1, &submitInfo, m_SwapChain->GetRenderCompleteFence()));
		}
	
	}
	
	namespace Native {
	
		Ref<NativeRendererAPI> NativeRendererAPI::Create(const Ref<NativeGraphicsContext> &context, const Ref<NativeSwapChain> &swapChain) {
			return MakeRef<Vulkan::VulkanRendererAPI>(
				CastRef<Vulkan::VulkanGraphicsContext>(context),
				CastRef<Vulkan::VulkanSwapChain>(swapChain)
			);
		}
	
	}
	
}