#pragma once

#ifdef _WIN32
#	define VK_USE_PLATFORM_WIN32_KHR
#	include <winsock2.h>
#endif

#include <vulkan/vulkan.h>

#include "./../../System/Memory.hpp"

#define JD_VULKAN_CALL(x) JD_ASSERT((x) == VK_SUCCESS, "%s failed", #x)
#define JD_VULKAN_LOAD_FUNC(instance, name) PFN_##name name = (PFN_##name)vkGetInstanceProcAddr(instance, #name)