#pragma once

#include "graphics/instance.hpp"
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
        std::optional<uint32_t> transferFamily;

        inline uint32_t* data() const {
            static uint32_t arr[] = {
                graphicsFamily.value(),
                presentFamily.value(),
                transferFamily.value()
            };

            return arr;
        }

        inline uint32_t size() const {
            return 3;
        }

        inline bool isComplete() const {
            return graphicsFamily.has_value() && 
                   presentFamily.has_value() && 
                   transferFamily.has_value();
        }

        inline bool hasDedicatedTransferFamiliy() const {
            return graphicsFamily.value() != transferFamily.value();
        }

        inline std::set<uint32_t> uniqueQueueFamilies() const {
            return {graphicsFamily.value(), presentFamily.value(), transferFamily.value()};
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
        Device(Instance& instance, Window& window);
        ~Device();

        Device(const Device&) = delete;
        Device& operator=(const Device&) = delete;
        Device(Device&&) = delete;
        Device& operator=(Device&&) = delete;

        void createBuffer(
            VkDeviceSize size, 
            VkBufferUsageFlags usage, 
            VkMemoryPropertyFlags properties, 
            VkBuffer* buffer, 
            VkDeviceMemory* bufferMemory);        

        void copyBuffer(
            VkDeviceSize size, 
            VkBuffer srcBuffer, 
            VkBuffer dstBuffer, 
            VkCommandBuffer commandBuffer);

        VkCommandBuffer beginTransferCommands();

        inline VkDevice device() const { return device_; }
        inline VkQueue graphicsQueue() const {return graphicsQueue_;}
        inline VkQueue presentQueue() const { return presentQueue_; }
        inline VkQueue transferQueue() const { return transferQueue_; }

        inline QueueFamilyIndices getPhysicalQueueFamilies() { return findQueueFamilies(physicalDevice_); }
        inline SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(physicalDevice_); }

    private:
        void pickPhysicalDevice();
        void createLogicalDevice();

        uint32_t ratePhysicalDeviceSuitability(VkPhysicalDevice physicalDevice);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice);
        bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice);

        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice);

        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

        Instance& instance;
        Window& window;

        VkPhysicalDevice physicalDevice_ = VK_NULL_HANDLE;
        VkDevice device_;
        VkQueue graphicsQueue_;
        VkQueue presentQueue_;
        VkQueue transferQueue_;
    };
}
