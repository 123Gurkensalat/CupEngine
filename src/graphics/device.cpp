#include "device.hpp"
#include "graphics/window.hpp"

#include <cstdint>
#include <map>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>

#include <vulkan/vulkan_core.h>

using namespace cup;

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

Device::Device(Instance& instance, Window& window) : instance(instance), window(window) 
{
    pickPhysicalDevice();
    createLogicalDevice();
}

Device::~Device() 
{
    vkDestroyDevice(device_, nullptr);
}

void Device::pickPhysicalDevice() 
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    std::multimap<uint32_t, VkPhysicalDevice> candidates{};

    // rate devices
    for (const auto& device : devices) {
        uint32_t score = ratePhysicalDeviceSuitability(device);
        candidates.insert(std::make_pair(score, device));
    }

    // select device
    if (candidates.rbegin()->first > 0) {
        physicalDevice_ = candidates.rbegin()->second;
    } else {
        throw std::runtime_error("failed to find a suitable GPU");
    }
}

uint32_t Device::ratePhysicalDeviceSuitability(VkPhysicalDevice physicalDevice)
{
    uint32_t score = 0;

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
    
    // required criteria
    if (!deviceFeatures.geometryShader)
        return 0;

    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
    if (!indices.isComplete())
        return 0;
    
    if (!checkDeviceExtensionSupport(physicalDevice))
        return 0;

    SwapChainSupportDetails swapChainDetails = querySwapChainSupport(physicalDevice);
    if (swapChainDetails.presentModes.empty() || swapChainDetails.formats.empty()) 
        return 0;

    // optional
    if (indices.presentFamily == indices.graphicsFamily)
        score += 10;

    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        score += 1000;

    score += deviceProperties.limits.maxImageDimension2D;

    return score;
}

QueueFamilyIndices Device::findQueueFamilies(VkPhysicalDevice physicalDevice)
{
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies{queueFamilyCount};
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    uint32_t i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, window.surface(), &presentSupport);

        if (presentSupport) {
            indices.presentFamily = i;
        }

        // dedicated transfer family
        if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT  && 
          !(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
            indices.transferFamily = i;
        }

        if (indices.isComplete()) break;

        i++;
    }

    if (!indices.transferFamily.has_value()) {
        indices.transferFamily = indices.graphicsFamily;
    }

    return indices;
}

bool Device::checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice) 
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions{extensionCount};
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
    
    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

SwapChainSupportDetails Device::querySwapChainSupport(VkPhysicalDevice physicalDevice) 
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, window.surface(), &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, window.surface(), &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, window.surface(), &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, window.surface(), &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, window.surface(), &presentModeCount, details.presentModes.data());
    }

    return details;
}

void Device::createLogicalDevice() {
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice_); 

    std::vector<VkDeviceQueueCreateInfo> queueInfos;
    
    float queuePriority = 1.0f; // ranges from 0 to 1. Where 1 has highest priority
    for (uint32_t queueFamily : indices.uniqueQueueFamilies()) {
        VkDeviceQueueCreateInfo queueInfo{};
        queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueInfo.queueFamilyIndex = queueFamily;
        queueInfo.queueCount = 1;
        queueInfo.pQueuePriorities = &queuePriority;
        queueInfos.push_back(queueInfo);
    }
    
    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueInfos.size());
    createInfo.pQueueCreateInfos = queueInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;
    
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (instance.validator().enableValidationLayers) {
        createInfo.enabledLayerCount = instance.validator().layerCount();
        createInfo.ppEnabledLayerNames = instance.validator().validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physicalDevice_, &createInfo, nullptr, &device_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device");
    }

    vkGetDeviceQueue(device_, indices.graphicsFamily.value(), 0, &graphicsQueue_);
    vkGetDeviceQueue(device_, indices.presentFamily.value(), 0, &presentQueue_);
    vkGetDeviceQueue(device_, indices.transferFamily.value(), 0, &transferQueue_);
}

void Device::createBuffer(
        VkDeviceSize size, 
        VkBufferUsageFlags usage, 
        VkMemoryPropertyFlags properties, 
        VkBuffer* buffer, 
        VkDeviceMemory* bufferMemory)
{
    auto queueFamily = getPhysicalQueueFamilies();
    uint32_t queueFamilyIndices[] = {queueFamily.graphicsFamily.value(), queueFamily.transferFamily.value()};
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;

    if (queueFamily.hasDedicatedTransferFamiliy()) {
        bufferInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
        bufferInfo.queueFamilyIndexCount = 2;
        bufferInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    if (vkCreateBuffer(device_, &bufferInfo, nullptr, buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vertex Buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device_, *buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device_, &allocInfo, nullptr, bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(device_, *buffer, *bufferMemory, 0);
}

void Device::copyBuffer(
        VkDeviceSize size, 
        VkBuffer srcBuffer, 
        VkBuffer dstBuffer, 
        VkCommandBuffer commandBuffer) 
{
    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
}

uint32_t Device::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) 
{
    VkPhysicalDeviceMemoryProperties memProperties{};
    vkGetPhysicalDeviceMemoryProperties(physicalDevice_, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && 
            (memProperties.memoryTypes[i].propertyFlags & properties) == properties) // properties match
        {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}
