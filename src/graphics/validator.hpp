#pragma once

#include <cstdint>
#include <vector>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

namespace cup {
    class Validator {
    public:
        void setupDebugMessenger(VkInstance instance);
        void cleanUpDebugMessenger(VkInstance instance);

        VkDebugUtilsMessengerCreateInfoEXT getDebugMessengerCreateInfo();

        std::vector<const char*> getRequiredExtensions();

        /// checks if required validation Layers are compatible
        /// returns if successfull
        bool checkValidationLayerSupport();

        inline uint32_t layerCount() { return static_cast<uint32_t>(validationLayers.size()); }

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);

        /// defaults to warning messages 
        static uint32_t minMessageSeverity;
        const std::vector<const char*> validationLayers = 
        {
            "VK_LAYER_KHRONOS_validation"
        };

    #ifdef NDEBUG
        const bool enableValidationLayers = false;
    #else
        const bool enableValidationLayers = true;
    #endif

    private:
        VkDebugUtilsMessengerEXT debugMessenger;
    };
}
