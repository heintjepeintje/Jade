#pragma once

#include "./../../Core.hpp"
#include "./Vulkan.hpp"
#include "./../NativeWindow.hpp"
#include "./../NativeGraphicsContext.hpp"

namespace Jade {

	namespace Vulkan {
		
		struct VulkanQueueFamilyIndices {
			uint32_t GraphicsFamilyIndex = UINT32_MAX;
			uint32_t ComputeFamilyIndex = UINT32_MAX;
			uint32_t TransferFamilyIndex = UINT32_MAX;
			uint32_t SparseBindingFamilyIndex = UINT32_MAX;
			uint32_t PresentIndex = UINT32_MAX;
		};
		
		class VulkanGraphicsContext : public Native::NativeGraphicsContext {
		public:
			VulkanGraphicsContext(const Ref<Native::NativeWindow> &window);
			~VulkanGraphicsContext();
			
			inline VkInstance GetInstance() { return m_Instance; }
			inline VkDevice GetLogicalDevice() { return m_LogicalDevice; }
			inline VkPhysicalDevice GetPhysicalDevice() { return m_PhysicalDevice; }
			inline VkSurfaceKHR GetSurface() { return m_Surface; }
			inline VkSurfaceCapabilitiesKHR GetSurfaceCapabilities() { return m_SurfaceCapabilities; }
			inline VkSurfaceFormatKHR GetSurfaceFormat() { return m_SurfaceFormat; }
			inline uint32_t GetGraphicsQueueIndex() { return m_QueueFamilyIndices.GraphicsFamilyIndex; }
			inline uint32_t GetPresentQueueIndex() { return m_QueueFamilyIndices.PresentIndex; }
			inline uint32_t GetTransferQueueIndex() { return m_QueueFamilyIndices.TransferFamilyIndex; }
			inline VkQueue GetTransferQueue() { return m_TransferQueue; }
			inline Ref<Native::NativeWindow> GetWindow() override { return m_Window; }
			
		private:
			VkInstance m_Instance = VK_NULL_HANDLE;
			VkDebugUtilsMessengerEXT m_DebugMessenger = VK_NULL_HANDLE;
			
			VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
			VkSurfaceCapabilitiesKHR m_SurfaceCapabilities{};
			VkSurfaceFormatKHR m_SurfaceFormat{};
			
			VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
			VkPhysicalDeviceProperties m_PhysicalDeviceProperties{};
			VkPhysicalDeviceFeatures m_PhysicalDeviceFeatures{};
			
			VulkanQueueFamilyIndices m_QueueFamilyIndices{};
			VkDevice m_LogicalDevice = VK_NULL_HANDLE;
			
			VkQueue m_TransferQueue = VK_NULL_HANDLE;
			
			Ref<Native::NativeWindow> m_Window;
		};
		
		class VulkanSwapChain : public Native::NativeSwapChain {
		public:
			VulkanSwapChain(const Ref<VulkanGraphicsContext> &context, uint32_t bufferCount);
			~VulkanSwapChain();
			
			virtual void Present() override;
			virtual void Resize(uint32_t width, uint32_t height) override;
			
			inline VkPresentModeKHR GetPresentMode() { return m_PresentMode; }
			inline VkSwapchainKHR GetSwapChain() { return m_SwapChain; }
			inline VkImage *GetImages() { return m_Images; }
			inline VkImage GetImage(uint32_t index) { return m_Images[index]; }
			inline VkImageView *GetImageViews() { return m_ImageViews; }
			inline VkImageView GetImageView(uint32_t index) { return m_ImageViews[index]; }
			inline VkFramebuffer GetFramebuffer(uint32_t index) { return m_Framebuffers[index]; }
			inline uint32_t GetImageCount() { return m_ImageCount; }
			inline VkQueue GetPresentQueue() { return m_PresentQueue; }
			inline VkCommandPool GetPresentCommandPool() { return m_PresentCommandPool; }
			inline VkCommandBuffer *GetPresentCommandBuffers() { return m_PresentCommandBuffers; }
			inline VkSemaphore GetPresentCompleteSemaphore() { return m_PresentCompleteSemaphore; }
			inline VkSemaphore GetRenderCompleteSemaphore() { return m_RenderCompleteSemaphore; }
			inline VkFence GetRenderCompleteFence() { return m_RenderCompleteFence; }
			inline VkFence GetPresentCompleteFence() { return m_PresentCompleteFence; }
			inline uint32_t GetNextImageIndex() { return m_NextImageIndex; }
			inline VkExtent2D GetSwapExtent() { return m_SwapExtent; } 
		
		private:
			VkPresentModeKHR m_PresentMode = VK_PRESENT_MODE_MAX_ENUM_KHR;
			VkExtent2D m_SwapExtent{};
			
			VkSwapchainKHR m_SwapChain = VK_NULL_HANDLE;
			VkImage *m_Images = nullptr;
			VkImageView *m_ImageViews = nullptr;
			uint32_t m_ImageCount = 0;
			
			VkRenderPass m_CompatibleRenderPass;
			VkFramebuffer *m_Framebuffers;
			
			VkQueue m_PresentQueue = VK_NULL_HANDLE;
			
			uint32_t m_NextImageIndex = 0;
			
			VkCommandPool m_PresentCommandPool = VK_NULL_HANDLE;
			VkCommandBuffer *m_PresentCommandBuffers = nullptr;
			
			VkSemaphore m_RenderCompleteSemaphore = VK_NULL_HANDLE;
			VkSemaphore m_PresentCompleteSemaphore = VK_NULL_HANDLE;
			VkFence m_RenderCompleteFence = VK_NULL_HANDLE;
			VkFence m_PresentCompleteFence = VK_NULL_HANDLE;
			
			Ref<VulkanGraphicsContext> m_Context;
		};
		
	};

}