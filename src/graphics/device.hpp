#pragma once

#include "validator.hpp"
#include "window.hpp"
#include <cstdint>
#include <optional>
#include <set>
#include <vulkan/vulkan_core.h>
namespace cup 
{
    struct QueueFamilyIndices 
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily; 

        inline uint32_t* data() {
            static uint32_t arr[] = {
                graphicsFamily.value(),
                presentFamily.value()
            };

            return arr;
        }

        inline bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }

        inline std::set<uint32_t> uniqueQueueFamilies() {
            return {graphicsFamily.value(), presentFamily.value()};
        }
    };

    struct SwapChainSupportDetails 
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class Device {
    public: 
        Device(Window& window);
        ~Device();

        Device(const Device &) = delete;
        Device& operator=(const Device &) = delete;
        Device(Device &&) = delete;
        Device &operator=(Device &&) = delete;

        void createBuffer(
            VkDeviceSize size, 
            VkBufferUsageFlags usage, 
            VkMemoryPropertyFlags properties, 
            VkBuffer* buffer, 
            VkDeviceMemory* bufferMemory);        

        inline VkDevice device() { return device_; }
        inline VkSurfaceKHR surface() { return surface_; }
        inline VkQueue graphicsQueue() {return graphicsQueue_;}
        inline VkQueue presentQueue() { return presentQueue_; }

        inline QueueFamilyIndices getPhysicalQueueFamilies() { return findQueueFamilies(physicalDevice_); }
        inline SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(physicalDevice_); }

    private:
        void createVkInstance();
        void pickPhysicalDevice();
        void createLogicalDevice();

        uint32_t ratePhysicalDeviceSuitability(VkPhysicalDevice physicalDevice);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice);
        bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice);

        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice);

        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

        Window& window;
        Validator validator{};

        VkInstance instance_;
        VkPhysicalDevice physicalDevice_ = VK_NULL_HANDLE;
        VkDevice device_;
        VkSurfaceKHR surface_;
        VkQueue graphicsQueue_;
        VkQueue presentQueue_;
    };
}
