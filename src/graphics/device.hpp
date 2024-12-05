#pragma once

#include "graphics/validator.hpp"
#include "graphics/window.hpp"
#include <cstdint>
#include <optional>
#include <vulkan/vulkan_core.h>
namespace cup 
{
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;

        inline bool isComplete() 
        {
            return graphicsFamily.has_value();
        }
    };


    class Device {
    public: 
        Device(Window& window);
        ~Device();

        Device(const Device &) = delete;
        Device& operator=(const Device &) = delete;
        Device(Device &&) = delete;
        Device &operator=(Device &&) = delete;

    private:
        void createVkInstance();
        void pickPhysicalDevice();
        void createLogicalDevice();
        uint32_t ratePhysicalDeviceSuitability(VkPhysicalDevice device);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);


        Window& window;
        Validator validator{};

        VkInstance instance;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device;
    };
}
