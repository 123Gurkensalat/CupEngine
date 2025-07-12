#pragma once

#include "utils/types.hpp"

#include <cstdint>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace cup {
class Validator : public NonCopyable {
public:
    Validator() = default;

    void setupDebugMessenger(VkInstance instance);
    void cleanUpDebugMessenger(VkInstance instance);

    VkDebugUtilsMessengerCreateInfoEXT getCreateInfo() const;

    std::vector<const char*> getRequiredExtensions() const;

    /// checks if required validation Layers are compatible
    /// returns if successfull
    bool checkValidationLayerSupport() const;

    inline static uint32_t minMessageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;

    const char* validationLayer = "VK_LAYER_KHRONOS_validation";

#ifdef NDEBUG
    inline static constexpr bool enableValidationLayers = false;
#else
    inline static constexpr bool enableValidationLayers = true;
#endif

private:
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);

    VkDebugUtilsMessengerEXT debugMessenger;
};
} // namespace cup
