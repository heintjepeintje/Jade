#include "./VulkanGraphicsContext.hpp"
#include <vector>
#include <cstring>

#ifdef _WIN32
#	include "./../Windows/WindowsWindow.hpp"
#endif // _WIN32

namespace Jade {

	namespace Vulkan {
		
		static void GetRequiredExtensions(std::vector<const char *> &extensions) {
			extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
		}
		
		static void CheckExtensionsAvailability(const std::vector<const char *> &extensions) {
			uint32_t availableExtensionCount = 0;
			vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);
			
			VkExtensionProperties *availableExtensions = Alloc<VkExtensionProperties>(availableExtensionCount);
			vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions);
			
			bool found = false;
			for (uint32_t i = 0; i < extensions.size(); i++) {
				const char *extensionName = extensions.at(i);
				for (uint32_t j = 0; j < availableExtensionCount; j++) {
					if (strcmp(availableExtensions[j].extensionName, extensionName) == 0) {
						JD_INFO("Found extension: %s", extensionName);
						found = true;
						break;
					}
				}
				JD_ASSERT(found, "Failed to find extension: %s", extensionName);
			}
			
			Free(availableExtensions);
		}
		
		static void GetRequiredLayers(std::vector<const char *> &layers) {
			layers.push_back("VK_LAYER_KHRONOS_validation");
		}
		
		static void CheckLayerAvailability(const std::vector<const char *> &layers) {
			uint32_t availableLayerCount = 0;
			vkEnumerateInstanceLayerProperties(&availableLayerCount, nullptr);
			
			VkLayerProperties *availableLayers = Alloc<VkLayerProperties>(availableLayerCount);
			vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers);
			
			bool found = false;
			for (uint32_t i = 0; i < layers.size(); i++) {
				const char *layerName = layers.at(i);
				for (uint32_t j = 0; j < availableLayerCount; j++) {
					if (strcmp(availableLayers[j].layerName, layerName) == 0) {
						JD_INFO("Found layer: %s", layerName);
						found = true;
						break;
					}
				}
				
				JD_ASSERT(found, "Failed to find layer: %s", layerName);
			}
			
			Free(availableLayers);
		}
		
		static VKAPI_ATTR VkBool32 VKAPI_CALL OnDebugMessage(
			VkDebugUtilsMessageSeverityFlagBitsEXT severity,
			VkDebugUtilsMessageTypeFlagsEXT type,
			const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
			void *userData JD_UNUSED
		) {
			if (type & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) {
				return VK_FALSE;
			}
			
			switch (severity) {
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: {
					JD_TRACE("VULKAN: %s", callbackData->pMessage);
					break;
				}
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: {
					JD_INFO("VULKAN: %s", callbackData->pMessage);
					break;
				}
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: {
					JD_WARN("VULKAN: %s", callbackData->pMessage);
					break;
				}
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: {
					JD_ERROR("VULKAN: %s", callbackData->pMessage);
					JD_DEBUGBREAK();
					break;
				}
				default: break;
			}
			return VK_FALSE;
		}
		
		static VkSurfaceKHR CreateSurface(VkInstance instance, const Ref<Native::NativeWindow> &window) {
			Ref<Windows::WindowsWindow> windowsWindow = std::dynamic_pointer_cast<Windows::WindowsWindow>(window);
			
			VkWin32SurfaceCreateInfoKHR surfaceInfo;
			surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			surfaceInfo.hwnd = windowsWindow->GetWindowHandle();
			surfaceInfo.hinstance = GetModuleHandle(NULL);
			surfaceInfo.flags = 0;
			surfaceInfo.pNext = nullptr;
			
			VkSurfaceKHR surface;
			JD_VULKAN_CALL(vkCreateWin32SurfaceKHR(instance, &surfaceInfo, nullptr, &surface));
			return surface;
		}
		
		static VkPhysicalDevice PickPhysicalDevice(uint32_t physicalDeviceCount, VkPhysicalDevice *physicalDevices, VkSurfaceKHR surface, VkPhysicalDeviceProperties *physicalDeviceProperties, VkPhysicalDeviceFeatures *physicalDeviceFeatures) {
			for (uint32_t i = 0; i < physicalDeviceCount; i++) {
				VkPhysicalDevice device = physicalDevices[i];
				
				VkPhysicalDeviceProperties properties;
				vkGetPhysicalDeviceProperties(device, &properties);
				
				VkPhysicalDeviceFeatures features;
				vkGetPhysicalDeviceFeatures(device, &features);
				
				JD_INFO("Checking device: %s", properties.deviceName);
				
				if (properties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
					JD_INFO("Device is not a discrete GPU. Skipping.");
					continue;
				}
				
				if (!features.geometryShader) {
					JD_INFO("Device has no geometry shader support. Skipping.");
					continue;
				}
					
				uint32_t queueCount = 0;
				vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, nullptr);
					
				VkQueueFamilyProperties *queues = Alloc<VkQueueFamilyProperties>(queueCount);
				vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, queues);
				
				bool hasGraphicsCapabilities = false;
				bool hasPresentCapabilities = false;
				
				for (uint32_t i = 0; i < queueCount; i++) {
					VkQueueFamilyProperties queue = queues[i];
					if (queue.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
						hasGraphicsCapabilities = true;
					}
					
					VkBool32 canPresent = VK_FALSE;
					JD_VULKAN_CALL(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &canPresent)); 
					
					if (canPresent) {
						hasPresentCapabilities = true;
					}
				}
				
				Free(queues);
				
				if (!hasGraphicsCapabilities) {
					JD_INFO("Device has no graphics capabilities. Skipping.");
					continue;
				}
				
				if (!hasPresentCapabilities) {
					JD_INFO("Device has no present capabilities. Skipping.");
					continue;
				}
				
				*physicalDeviceProperties = properties;
				*physicalDeviceFeatures = features;
				
				return device;
			}
			
			JD_WARN("Could not find a valid physical device!");
				
			return VK_NULL_HANDLE;
		}
		
		static VulkanQueueFamilyIndices GetDeviceQueueIndices(VkPhysicalDevice device, VkSurfaceKHR surface) {
			VulkanQueueFamilyIndices queueFamilies;
			
			uint32_t queueCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, nullptr);
				
			VkQueueFamilyProperties *queues = Alloc<VkQueueFamilyProperties>(queueCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, queues);
						
			for (uint32_t i = 0; i < queueCount; i++) {
				VkQueueFamilyProperties queue = queues[i];
				if (queue.queueFlags & VK_QUEUE_GRAPHICS_BIT && queueFamilies.GraphicsFamilyIndex == UINT32_MAX) {
					queueFamilies.GraphicsFamilyIndex = i;
					continue;
				}
				
				if (queueFamilies.PresentIndex == UINT32_MAX) {
					VkBool32 hasPresentCapabilities = VK_FALSE;
					vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &hasPresentCapabilities);
					if (hasPresentCapabilities) {
						queueFamilies.PresentIndex = i;
					}
				}
				
				if (queue.queueFlags & VK_QUEUE_COMPUTE_BIT && queueFamilies.ComputeFamilyIndex == UINT32_MAX) {
					queueFamilies.ComputeFamilyIndex = i;
					continue;
				}
				
				if (queue.queueFlags & VK_QUEUE_TRANSFER_BIT && queueFamilies.TransferFamilyIndex == UINT32_MAX) {
					queueFamilies.TransferFamilyIndex = i;
				}
				
				if (queue.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT && queueFamilies.SparseBindingFamilyIndex == UINT32_MAX) {
					queueFamilies.SparseBindingFamilyIndex = i;
				}
			}
			
			Free(queues);
			
			return queueFamilies;
		}
		
		static void GetRequiredDeviceExtensions(std::vector<const char *> &extensions) {
			extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		}
		
		static void CheckDeviceExtensionAvailability(VkPhysicalDevice device, const std::vector<const char *> &extensions) {
			uint32_t availableDeviceExtensionCount = 0;
			JD_VULKAN_CALL(vkEnumerateDeviceExtensionProperties(device, nullptr, &availableDeviceExtensionCount, nullptr));
			
			VkExtensionProperties *availableDeviceExtensions = Alloc<VkExtensionProperties>(availableDeviceExtensionCount);
			JD_VULKAN_CALL(vkEnumerateDeviceExtensionProperties(device, nullptr, &availableDeviceExtensionCount, availableDeviceExtensions));
		
			bool found = true;
			for (uint32_t i = 0; i < extensions.size(); i++) {
				const char *extensionName = extensions.at(i);
				for (uint32_t j = 0; j < availableDeviceExtensionCount; j++) {
					if (strcmp(availableDeviceExtensions[j].extensionName, extensionName) == 0) {
						break;
					}
				}
				
				if (!found) {
					JD_ASSERT(found, "Failed to find device extension: %s", extensionName);
				}
			}
			
			Free(availableDeviceExtensions);
		}
		
		static VkSurfaceFormatKHR PickSurfaceFormat(uint32_t surfaceFormatCount, const VkSurfaceFormatKHR *surfaceFormats) {
			for (uint32_t i = 0; i < surfaceFormatCount; i++) {
				VkSurfaceFormatKHR format = surfaceFormats[i];
				
				if (format.format == VK_FORMAT_R8G8B8A8_UNORM) {
					return format;
				}
			}
			return surfaceFormats[0];
		}
		
		static VkPresentModeKHR PickPresentMode(uint32_t presentModeCount, const VkPresentModeKHR *presentModes) {
			for (uint32_t i = 0; i < presentModeCount; i++) {
				VkPresentModeKHR presentMode = presentModes[i];
				
				if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
					return presentMode;
				}
			}
			return VK_PRESENT_MODE_FIFO_KHR;
		}
		
		static VkExtent2D PickSwapExtent(const Ref<Native::NativeWindow> &window, const VkSurfaceCapabilitiesKHR &capabilities) {
			if (capabilities.currentExtent.width != UINT32_MAX) {
				return capabilities.currentExtent;
			}
			
			VkExtent2D extent;
			
			extent.width = std::clamp(window->GetWidth(), capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			extent.height = std::clamp(window->GetHeight(), capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
			
			return extent;
		}
	
		VulkanGraphicsContext::VulkanGraphicsContext(const Ref<Native::NativeWindow> &window) : m_Window(window) {
			VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo;
			debugMessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debugMessengerInfo.flags = 0;
			debugMessengerInfo.pfnUserCallback = OnDebugMessage;
			debugMessengerInfo.pNext = nullptr;
			debugMessengerInfo.pUserData = nullptr;
			
			debugMessengerInfo.messageSeverity = 0
				| VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT 
				| VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
			;
			debugMessengerInfo.messageType = 0
				| VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
			;
			
			VkApplicationInfo appInfo;
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = window->GetTitle().c_str();
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName = "Lumix Engine";
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_3; // Latest api version
			appInfo.pNext = NULL; // No extension information
			
			VkInstanceCreateInfo instanceInfo;
			instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			instanceInfo.flags = 0;
			instanceInfo.pApplicationInfo = &appInfo;
			instanceInfo.pNext = (void*)&debugMessengerInfo;
			
			std::vector<const char *> requiredExtensions;
			GetRequiredExtensions(requiredExtensions);			
			CheckExtensionsAvailability(requiredExtensions);
			
			instanceInfo.enabledExtensionCount = requiredExtensions.size();
			instanceInfo.ppEnabledExtensionNames = requiredExtensions.data();
			
			std::vector<const char *> requiredLayers;
			GetRequiredLayers(requiredLayers);
			CheckLayerAvailability(requiredLayers);
			
			instanceInfo.enabledLayerCount = requiredLayers.size();
			instanceInfo.ppEnabledLayerNames = requiredLayers.data();
			
			JD_VULKAN_CALL(vkCreateInstance(&instanceInfo, nullptr, &m_Instance));
			
			JD_VULKAN_LOAD_FUNC(m_Instance, vkCreateDebugUtilsMessengerEXT);
			JD_VULKAN_CALL(vkCreateDebugUtilsMessengerEXT(m_Instance, &debugMessengerInfo, nullptr, &m_DebugMessenger));
			
			uint32_t physicalDeviceCount = 0;
			JD_VULKAN_CALL(vkEnumeratePhysicalDevices(m_Instance, &physicalDeviceCount, NULL));
			
			VkPhysicalDevice *physicalDevices = Alloc<VkPhysicalDevice>(physicalDeviceCount);
			JD_VULKAN_CALL(vkEnumeratePhysicalDevices(m_Instance, &physicalDeviceCount, physicalDevices));
			
			m_Surface = CreateSurface(m_Instance, window);
			
			m_PhysicalDevice = PickPhysicalDevice(physicalDeviceCount, physicalDevices, m_Surface, &m_PhysicalDeviceProperties, &m_PhysicalDeviceFeatures);
			
			Free(physicalDevices);
			
			m_QueueFamilyIndices = GetDeviceQueueIndices(m_PhysicalDevice, m_Surface); 
			
			JD_VULKAN_CALL(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
				m_PhysicalDevice, 
				m_Surface, 
				&m_SurfaceCapabilities
			));
			
			uint32_t surfaceFormatCount = 0;
			JD_VULKAN_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, m_Surface, &surfaceFormatCount, nullptr));
			
			VkSurfaceFormatKHR *surfaceFormats = Alloc<VkSurfaceFormatKHR>(surfaceFormatCount);
			JD_VULKAN_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, m_Surface, &surfaceFormatCount, surfaceFormats));
			
			m_SurfaceFormat = PickSurfaceFormat(surfaceFormatCount, surfaceFormats);
			Free(surfaceFormats);
			
			float defaultPriority = 1.0f;
			
			VkDeviceQueueCreateInfo queueInfos[] = {
				{
					.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
					.pNext = nullptr,
					.flags = 0,
					.queueFamilyIndex = m_QueueFamilyIndices.GraphicsFamilyIndex,
					.queueCount = 1,
					.pQueuePriorities = &defaultPriority,
				},
				{
					.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
					.pNext = nullptr,
					.flags = 0,
					.queueFamilyIndex = m_QueueFamilyIndices.PresentIndex,
					.queueCount = 1,
					.pQueuePriorities = &defaultPriority,
				},
				{
					.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
					.pNext = nullptr,
					.flags = 0,
					.queueFamilyIndex = m_QueueFamilyIndices.TransferFamilyIndex,
					.queueCount = 1,
					.pQueuePriorities = &defaultPriority,
				},
			};
			
			std::vector<const char *> requiredDeviceExtensions;
			GetRequiredDeviceExtensions(requiredDeviceExtensions);
			CheckDeviceExtensionAvailability(m_PhysicalDevice, requiredDeviceExtensions);
			
			VkDeviceCreateInfo deviceInfo = {
				.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.queueCreateInfoCount = JD_ARRAYSIZE(queueInfos),
				.pQueueCreateInfos = queueInfos,
				.enabledLayerCount = (uint32_t)requiredLayers.size(),
				.ppEnabledLayerNames = requiredLayers.data(),
				.enabledExtensionCount = (uint32_t)requiredDeviceExtensions.size(),
				.ppEnabledExtensionNames = requiredDeviceExtensions.data(),
				.pEnabledFeatures = &m_PhysicalDeviceFeatures,
			};
			
			
			JD_VULKAN_CALL(vkCreateDevice(m_PhysicalDevice, &deviceInfo, nullptr, &m_LogicalDevice));
		}
		
		VulkanGraphicsContext::~VulkanGraphicsContext() {
			JD_VULKAN_CALL(vkDeviceWaitIdle(m_LogicalDevice));
			
			vkDestroyDevice(m_LogicalDevice, nullptr);
			vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
		
			JD_VULKAN_LOAD_FUNC(m_Instance, vkDestroyDebugUtilsMessengerEXT);
			vkDestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
			vkDestroyInstance(m_Instance, nullptr);
		}
		
		VulkanSwapChain::VulkanSwapChain(const Ref<VulkanGraphicsContext> &context, uint32_t bufferCount) : m_Context(context) {
			vkGetDeviceQueue(
				m_Context->GetLogicalDevice(), 
				m_Context->GetPresentQueueIndex(), 
				0, 
				&m_PresentQueue
			);
			
			uint32_t presentModeCount = 0;
			JD_VULKAN_CALL(vkGetPhysicalDeviceSurfacePresentModesKHR(
				m_Context->GetPhysicalDevice(), 
				m_Context->GetSurface(), 
				&presentModeCount, 
				nullptr
			));
			
			VkPresentModeKHR *presentModes = Alloc<VkPresentModeKHR>(presentModeCount);
			JD_VULKAN_CALL(vkGetPhysicalDeviceSurfacePresentModesKHR(
				m_Context->GetPhysicalDevice(), 
				m_Context->GetSurface(), 
				&presentModeCount, 
				presentModes
			));
			
			m_PresentMode = PickPresentMode(presentModeCount, presentModes);
			Free(presentModes);
			
			m_SwapExtent = PickSwapExtent(
				m_Context->GetWindow(), 
				m_Context->GetSurfaceCapabilities()
			);
			
			uint32_t imageCount = std::clamp(
				bufferCount,
				m_Context->GetSurfaceCapabilities().minImageCount, 
				m_Context->GetSurfaceCapabilities().maxImageCount);
			
			uint32_t queueFamilyIndices[] = { m_Context->GetGraphicsQueueIndex(), m_Context->GetPresentQueueIndex() };
			
			VkSwapchainCreateInfoKHR swapChainInfo = {
				.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
				.pNext = nullptr,
				.flags = 0,
				.surface = m_Context->GetSurface(),
				.minImageCount = imageCount,
				.imageFormat = m_Context->GetSurfaceFormat().format,
				.imageColorSpace = m_Context->GetSurfaceFormat().colorSpace,
				.imageExtent = m_SwapExtent,
				.imageArrayLayers = 1,
				.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
				.imageSharingMode = VK_SHARING_MODE_CONCURRENT,
				.queueFamilyIndexCount = JD_ARRAYSIZE(queueFamilyIndices),
				.pQueueFamilyIndices = queueFamilyIndices,
				.preTransform = m_Context->GetSurfaceCapabilities().currentTransform,
				.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
				.presentMode = m_PresentMode,
				.clipped = VK_TRUE,
				.oldSwapchain = nullptr,
			};
			
			JD_VULKAN_CALL(vkCreateSwapchainKHR(
				m_Context->GetLogicalDevice(), 
				&swapChainInfo, 
				nullptr, 
				&m_SwapChain
			));
			
			JD_VULKAN_CALL(vkGetSwapchainImagesKHR(
				m_Context->GetLogicalDevice(), 
				m_SwapChain,
				&m_ImageCount, 
				nullptr
			));
			
			m_Images = Alloc<VkImage>(m_ImageCount);
			m_ImageViews = Alloc<VkImageView>(m_ImageCount);
			m_Framebuffers = Alloc<VkFramebuffer>(m_ImageCount);
			
			JD_VULKAN_CALL(vkGetSwapchainImagesKHR(
				m_Context->GetLogicalDevice(), 
				m_SwapChain, 
				&m_ImageCount, 
				m_Images
			));
			
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
				&m_CompatibleRenderPass
			));
			
			for (uint32_t i = 0; i < m_ImageCount; i++) {
				VkImageViewCreateInfo imageViewInfo = {
					.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
					.pNext = nullptr,
					.flags = 0,
					.image = m_Images[i],
					.viewType = VK_IMAGE_VIEW_TYPE_2D,
					.format = m_Context->GetSurfaceFormat().format,
					.components = {
						.r = VK_COMPONENT_SWIZZLE_IDENTITY,
						.g = VK_COMPONENT_SWIZZLE_IDENTITY,	
						.b = VK_COMPONENT_SWIZZLE_IDENTITY,
						.a = VK_COMPONENT_SWIZZLE_IDENTITY,
					},
				
					.subresourceRange = {
						.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
						.baseMipLevel = 0,
						.levelCount = 1,
						.baseArrayLayer = 0,
						.layerCount = 1,
					}
				};
				
				JD_VULKAN_CALL(vkCreateImageView(
					m_Context->GetLogicalDevice(), 
					&imageViewInfo, 
					nullptr, 
					&m_ImageViews[i]
				));
				
				VkImageView imageViews[] = { m_ImageViews[i] };
				
				VkFramebufferCreateInfo framebufferInfo = {
					.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
					.pNext = nullptr,
					.flags = 0,
					.renderPass = m_CompatibleRenderPass,
					.attachmentCount = 1,
					.pAttachments = imageViews,
					.width = m_SwapExtent.width,
					.height = m_SwapExtent.height,
					.layers = 1,
				};
				
				JD_VULKAN_CALL(vkCreateFramebuffer(
					m_Context->GetLogicalDevice(),
					&framebufferInfo,
					nullptr,
					&m_Framebuffers[i]
				));
			}
			
			VkCommandPoolCreateInfo presentCommandPoolInfo = {
				.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.queueFamilyIndex = m_Context->GetPresentQueueIndex()
			};
			
			JD_VULKAN_CALL(vkCreateCommandPool(
				m_Context->GetLogicalDevice(), 
				&presentCommandPoolInfo, 
				nullptr, 
				&m_PresentCommandPool
			));
			
			m_PresentCommandBuffers = Alloc<VkCommandBuffer>(m_ImageCount);
			
			VkCommandBufferAllocateInfo presentCommandBufferInfo = {
				.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
				.pNext = nullptr,
				.commandPool = m_PresentCommandPool,
				.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
				.commandBufferCount = m_ImageCount
			};
			
			JD_VULKAN_CALL(vkAllocateCommandBuffers(
				m_Context->GetLogicalDevice(), 
				&presentCommandBufferInfo, 
				m_PresentCommandBuffers
			));
			
			VkSemaphoreCreateInfo semaphoreInfo = {
				.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0
			};
			
			JD_VULKAN_CALL(vkCreateSemaphore(
				m_Context->GetLogicalDevice(), 
				&semaphoreInfo, 
				nullptr, 
				&m_PresentCompleteSemaphore
			));
			
			JD_VULKAN_CALL(vkCreateSemaphore(
				m_Context->GetLogicalDevice(), 
				&semaphoreInfo, 
				nullptr, 
				&m_RenderCompleteSemaphore
			));
			
			VkFenceCreateInfo fenceInfo = {
				.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0
			};
			
			JD_VULKAN_CALL(vkCreateFence(
				m_Context->GetLogicalDevice(),
				&fenceInfo,
				nullptr,
				&m_RenderCompleteFence
			));
			
			JD_VULKAN_CALL(vkCreateFence(
				m_Context->GetLogicalDevice(),
				&fenceInfo,
				nullptr,
				&m_PresentCompleteFence
			));
			
			JD_VULKAN_CALL(vkAcquireNextImageKHR(
				m_Context->GetLogicalDevice(),
				m_SwapChain,
				UINT64_MAX,
				m_PresentCompleteSemaphore,
				m_PresentCompleteFence,
				&m_NextImageIndex
			));
		}
		
		VulkanSwapChain::~VulkanSwapChain() {
			JD_VULKAN_CALL(vkQueueWaitIdle(m_PresentQueue));
			
			vkDestroySemaphore(
				m_Context->GetLogicalDevice(), 
				m_PresentCompleteSemaphore, 
				nullptr
			);
			
			vkDestroySemaphore(
				m_Context->GetLogicalDevice(), 
				m_RenderCompleteSemaphore,
				nullptr
			);
			
			vkDestroyFence(
				m_Context->GetLogicalDevice(),
				m_RenderCompleteFence,
				nullptr
			);
			
			vkDestroyFence(
				m_Context->GetLogicalDevice(),
				m_PresentCompleteFence,
				nullptr
			);
			
			vkFreeCommandBuffers(
				m_Context->GetLogicalDevice(), 
				m_PresentCommandPool, 
				m_ImageCount, 
				m_PresentCommandBuffers
			);
			
			vkDestroyCommandPool(
				m_Context->GetLogicalDevice(), 
				m_PresentCommandPool, 
				nullptr
			);
			for (uint32_t i = 0; i < m_ImageCount; i++) {
				vkDestroyImageView(
					m_Context->GetLogicalDevice(), 
					m_ImageViews[i],
					nullptr
				);
				
				vkDestroyFramebuffer(
					m_Context->GetLogicalDevice(),
					m_Framebuffers[i],
					nullptr
				);
			}
			
			vkDestroyRenderPass(
				m_Context->GetLogicalDevice(),
				m_CompatibleRenderPass,
				nullptr
			);
			
			Free(m_ImageViews);
			Free(m_Images);
			
			vkDestroySwapchainKHR(
				m_Context->GetLogicalDevice(),
				m_SwapChain,
				nullptr
			);
		}
		
		void VulkanSwapChain::Present() {
			VkFence waitFences[] = { m_RenderCompleteFence };
			
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
			
			VkResult result = VK_SUCCESS;
			
			VkPresentInfoKHR presentInfo = {
				.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
				.pNext = nullptr,
				.waitSemaphoreCount = 1,
				.pWaitSemaphores = &m_RenderCompleteSemaphore,
				.swapchainCount = 1,
				.pSwapchains = &m_SwapChain,
				.pImageIndices = &m_NextImageIndex,
				.pResults = &result
			};
			
			VkResult presentResult = vkQueuePresentKHR(m_PresentQueue, &presentInfo);
			
			if (presentResult == VK_ERROR_OUT_OF_DATE_KHR) {
				Resize(m_Context->GetWindow()->GetWidth(), m_Context->GetWindow()->GetHeight());
				return;
			}
			
			JD_VULKAN_CALL(result);
			
			JD_VULKAN_CALL(vkAcquireNextImageKHR(
				m_Context->GetLogicalDevice(),
				m_SwapChain,
				UINT64_MAX,
				m_PresentCompleteSemaphore,
				m_PresentCompleteFence,
				&m_NextImageIndex
			));
		}
		
		void VulkanSwapChain::Resize(uint32_t width JD_UNUSED, uint32_t height JD_UNUSED) {
			
		}
	
	}
	
	namespace Native {
	
		Ref<NativeGraphicsContext> NativeGraphicsContext::Create(const Ref<NativeWindow> &window) {
			return std::make_shared<Vulkan::VulkanGraphicsContext>(
				window
			);
		}
		
		Ref<NativeSwapChain> NativeSwapChain::Create(const Ref<NativeGraphicsContext> &context, uint32_t bufferCount) {
			return std::make_shared<Vulkan::VulkanSwapChain>(
				std::dynamic_pointer_cast<Vulkan::VulkanGraphicsContext>(context), 
				bufferCount
			);
		}
	
	}

}