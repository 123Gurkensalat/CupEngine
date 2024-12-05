#pragma once

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
        const bool enableValidationLayers = true;
    #else
        const bool enableValidationLayers = false;
    #endif

    private:
        VkDebugUtilsMessengerEXT debugMessenger;
    };
}
