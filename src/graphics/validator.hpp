#pragma once

#include <vulkan/vulkan_core.h>

#include <cstdint>
#include <vector>

namespace cup {
    class Validator {
    public:
        Validator() = default;
        Validator(const Validator&) = delete;
        Validator& operator=(const Validator&) = delete;

        void setupDebugMessenger(VkInstance instance);
        void cleanUpDebugMessenger(VkInstance instance);

        VkDebugUtilsMessengerCreateInfoEXT getCreateInfo();

        std::vector<const char*> getRequiredExtensions();

        /// checks if required validation Layers are compatible
        /// returns if successfull
        bool checkValidationLayerSupport();

        inline uint32_t layerCount() { return static_cast<uint32_t>(validationLayers.size()); }

        /// defaults to warning messages 
        static uint32_t minMessageSeverity;
        const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

    #ifdef NDEBUG
        const bool enableValidationLayers = false;
    #else
        const bool enableValidationLayers = true;
    #endif

    private:
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);

        VkDebugUtilsMessengerEXT debugMessenger;
    };
}
