#pragma once

#include <stdexcept>
#include <string_view>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
namespace cup {

#define TYPE_LIST                                                                                  \
    X(VkBuffer, VK_OBJECT_TYPE_BUFFER)                                                             \
    X(VkCommandPool, VK_OBJECT_TYPE_COMMAND_POOL)

template<typename T>
struct VulkanObjectType;

#define X(type, enum)                                                                              \
    template<>                                                                                     \
    struct VulkanObjectType<type> {                                                                \
        static constexpr VkObjectType value = enum;                                                \
    };
TYPE_LIST
#undef X
#undef TYPE_LIST

class DebugUtils {
#ifndef NDEBUG
public:
    DebugUtils() = delete;

    static void init(VkInstance instance, VkDevice device)
    {
        vkSetDebugUtilsObjectNameEXT = reinterpret_cast<PFN_vkSetDebugUtilsObjectNameEXT>(
            vkGetInstanceProcAddr(instance, "vkSetDebugUtilsObjectNameEXT"));
        vkCmdBeginDebugUtilsLabelEXT = reinterpret_cast<PFN_vkCmdBeginDebugUtilsLabelEXT>(
            vkGetInstanceProcAddr(instance, "vkCmdBeginDebugUtilsLabelEXT"));
        vkCmdEndDebugUtilsLabelEXT = reinterpret_cast<PFN_vkCmdEndDebugUtilsLabelEXT>(
            vkGetInstanceProcAddr(instance, "vkCmdEndDebugUtilsLabelEXT"));
        vkCmdInsertDebugUtilsLabelEXT = reinterpret_cast<PFN_vkCmdInsertDebugUtilsLabelEXT>(
            vkGetInstanceProcAddr(instance, "vkCmdInsertDebugUtilsLabelEXT"));

        if (!vkSetDebugUtilsObjectNameEXT || !vkCmdBeginDebugUtilsLabelEXT ||
            !vkCmdEndDebugUtilsLabelEXT || !vkCmdInsertDebugUtilsLabelEXT) {
            throw std::runtime_error("Could not find ProcAddress of debug functions");
        }
        s_device = device;
    }

    // Set the name of any Vulkan object
    template<typename T>
    static void setObjectName(T object, std::string_view name)
    {
        VkDebugUtilsObjectNameInfoEXT nameInfo{
            .objectType   = VulkanObjectType<T>::value,
            .objectHandle = reinterpret_cast<uint64_t>(object),
            .pObjectName  = name.data(),
        };

        vkSetDebugUtilsObjectNameEXT(s_device, &nameInfo);
    }

    // Insert a single debug label
    static void insertLabel(
        VkCommandBuffer cmd,
        std::string_view labelName,
        float r = 1.0f,
        float g = 1.0f,
        float b = 1.0f,
        float a = 1.0f)
    {
        VkDebugUtilsLabelEXT labelInfo{
            .pLabelName = labelName.data(),
            .color      = {r, g, b, a},
        };

        vkCmdInsertDebugUtilsLabelEXT(cmd, &labelInfo);
    }

    // Begin/End label regions (for nesting)
    static void beginLabel(
        VkCommandBuffer cmd,
        std::string_view labelName,
        float r = 1.0f,
        float g = 1.0f,
        float b = 1.0f,
        float a = 1.0f)
    {
        if (!vkCmdBeginDebugUtilsLabelEXT)
            return;

        VkDebugUtilsLabelEXT labelInfo{
            .pLabelName = labelName.data(),
            .color      = {r, g, b, a},
        };

        vkCmdBeginDebugUtilsLabelEXT(cmd, &labelInfo);
    }

    static void endLabel(VkCommandBuffer cmd)
    {
        if (!vkCmdEndDebugUtilsLabelEXT)
            return;
        vkCmdEndDebugUtilsLabelEXT(cmd);
    }

private:
    inline static PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT   = nullptr;
    inline static PFN_vkCmdBeginDebugUtilsLabelEXT vkCmdBeginDebugUtilsLabelEXT   = nullptr;
    inline static PFN_vkCmdEndDebugUtilsLabelEXT vkCmdEndDebugUtilsLabelEXT       = nullptr;
    inline static PFN_vkCmdInsertDebugUtilsLabelEXT vkCmdInsertDebugUtilsLabelEXT = nullptr;

    inline static VkDevice s_device = VK_NULL_HANDLE;
#else
public:
    static inline void init(VkInstance, VkDevice) {}
    static inline void setObjectName(void*, std::string_view) {}
    static inline void
    insertLabel(VkCommandBuffer, std::string_view, float = 1, float = 1, float = 1, float = 1)
    {}
    static inline void
    beginLabel(VkCommandBuffer, std::string_view, float = 1, float = 1, float = 1, float = 1)
    {}
    static inline void endLabel(VkCommandBuffer) {}
#endif
};
} // namespace cup
