#pragma once

#include "instance.hpp"
#include "utils/types.hpp"
#include "window.hpp"

#include <cstdint>
#include <optional>
#include <set>
#include <string_view>
#include <vulkan/vulkan_core.h>

namespace cup {
struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    std::optional<uint32_t> transferFamily;

    uint32_t* data() const
    {
        static uint32_t arr[] = {
            graphicsFamily.value(),
            presentFamily.value(),
            transferFamily.value()};

        return arr;
    }

    uint32_t size() const { return 3; }

    bool isComplete() const
    {
        return graphicsFamily.has_value() && presentFamily.has_value() &&
               transferFamily.has_value();
    }

    bool hasDedicatedTransferFamiliy() const
    {
        return graphicsFamily.value() != transferFamily.value();
    }

    std::set<uint32_t> uniqueQueueFamilies() const
    {
        return {graphicsFamily.value(), presentFamily.value(), transferFamily.value()};
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class Device : public NonCopyable, NonMovable {
public:
    Device(Instance& instance);
    ~Device();

    void createBuffer(
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties,
        VkBuffer* buffer,
        VkDeviceMemory* bufferMemory,
        std::string_view name = {}) const;

    void createTransferBuffer(
        VkDeviceSize size,
        const void* data,
        VkBuffer* dstBuffer,
        VkDeviceMemory* dstBufferMemory,
        VkBufferUsageFlags usage,
        std::string_view name = {}) const;

    void copyBuffer(VkDeviceSize size, VkBuffer srcBuffer, VkBuffer dstBuffer) const;

    void createImage(
        VkExtent3D extent,
        VkFormat format,
        VkImageTiling tiling,
        VkImageUsageFlags usage,
        VkMemoryPropertyFlags properties,
        VkImage* image,
        VkDeviceMemory* imageMemory,
        std::string_view name = {}) const;

    void createImageView(
        VkImage image, VkFormat format, VkImageView* imageView, std::string_view name = {}) const;

    VkCommandBuffer beginTransferCommands(std::string_view name = {}) const;
    void endTransferCommands(VkCommandBuffer commandBuffer) const;

    VkDevice device() const { return device_; }

    const VkQueue graphicsQueue() const { return graphicsQueue_; }
    const VkQueue presentQueue() const { return presentQueue_; }
    const VkQueue transferQueue() const { return transferQueue_; }

    const VkCommandPool graphicsCommandPool() const { return graphicsCommandPool_; }
    const VkCommandPool transferCommandPool() const { return transferCommandPool_; }
    const VkDescriptorPool descriptorPool() const { return descriptorPool_; }

    // TODO: add const info member
    // TODO: Move to DeviceBuilder. Those will be available with the info member
    const VkPhysicalDeviceProperties properties() const
    {
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(physicalDevice_, &props);
        return props;
    }

    const VkPhysicalDeviceLimits limits() const { return properties().limits; }

    std::unordered_set<std::string_view> getExtensions();
    const QueueFamilyIndices physicalQueueFamilies() const
    {
        return findQueueFamilies(physicalDevice_);
    }
    SwapChainSupportDetails swapChainSupport() const
    {
        return querySwapChainSupport(physicalDevice_);
    }

private:
    void createLogicalDevice();
    void createCommandPool(
        uint32_t queueFamilyIndex, VkCommandPool* commandPool, std::string_view name = {});
    void createDescriptorPool(const char* name);

    uint32_t ratePhysicalDeviceSuitability(VkPhysicalDevice physicalDevice) const;
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice) const;
    bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice) const;

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice) const;

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

    VkPhysicalDevice physicalDevice_;
    VkDevice device_;

    VkQueue graphicsQueue_;
    VkQueue presentQueue_;
    VkQueue transferQueue_;

    VkCommandPool graphicsCommandPool_;
    VkCommandPool transferCommandPool_;
    VkDescriptorPool descriptorPool_;
};

class DeviceBuilder {
    std::unordered_set<std::string_view> req_extensions{};
    std::unordered_set<std::string_view> opt_extensions{};

    void pickPhysicalDevice();

public:
    void addRequiredExtension(std::string_view name);
    void addRequiredExtensions(std::vector<std::string_view>&& names);
    void addOptionalExtension(std::string_view name);
    void addOptionalExtensions(std::vector<std::string_view>&& names);

    Device build(Instance& instance, Window& window);
};

} // namespace cup
