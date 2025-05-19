#pragma once

#include <vulkan/vulkan_core.h>

#define VK_DEBUG_NAME_OBJECT () \
    VkDebugUtilsObjectNameInfoEXT nameInfo{}; \
    nameInfo.stype = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT; \
    nameInfo.objectType = VK_OBJECT_TYPE_YOUR_OBJECT; \
    nameInfo.objectHandle = static_cast<uint64_t>(buffer); \


