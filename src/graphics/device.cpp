#include "device.hpp"
#include "graphics/window.hpp"

#include <cstdint>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>

#include <vulkan/vulkan_core.h>

using cup::Device;
using cup::QueueFamilyIndices;
using cup::SwapChainSupportDetails;

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

Device::Device(cup::Window& window) : window(window) 
{
    createVkInstance();
    validator.setupDebugMessenger(instance_);
    window.createSurface(instance_, &surface_);
    pickPhysicalDevice();
    createLogicalDevice();
}

Device::~Device() 
{
    vkDestroyDevice(device_, nullptr);
    validator.cleanUpDebugMessenger(instance_);
    vkDestroySurfaceKHR(instance_, surface_, nullptr);
    vkDestroyInstance(instance_, nullptr);
}

void Device::createVkInstance() 
{
    if (!validator.checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "ApplicationName";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "CupEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    uint32_t availableExtensionsCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionsCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions{availableExtensionsCount};
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionsCount, availableExtensions.data());

    std::cout << "available extensions:\n";
    for(const auto& extension : availableExtensions) {
        std::cout << '\t' << extension.extensionName << '\n';
    }

    auto extensions = validator.getRequiredExtensions();

    auto debugInfo = validator.getCreateInfo();

    VkInstanceCreateInfo instanceInfo{};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    instanceInfo.ppEnabledExtensionNames = extensions.data();
    
    if(validator.enableValidationLayers) {
        instanceInfo.enabledLayerCount = static_cast<uint32_t>(validator.validationLayers.size());
        instanceInfo.ppEnabledLayerNames = validator.validationLayers.data();

        instanceInfo.pNext = &debugInfo;
    } else {
        instanceInfo.enabledLayerCount = 0;
    }

    if (vkCreateInstance(&instanceInfo, nullptr, &instance_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}

void Device::pickPhysicalDevice() 
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance_, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance_, &deviceCount, devices.data());

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

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface_, &presentSupport);

        if (presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) break;

        i++;
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

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface_, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface_, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface_, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface_, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface_, &presentModeCount, details.presentModes.data());
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

    if (validator.enableValidationLayers) {
        createInfo.enabledLayerCount = validator.layerCount();
        createInfo.ppEnabledLayerNames = validator.validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physicalDevice_, &createInfo, nullptr, &device_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device");
    }

    vkGetDeviceQueue(device_, indices.graphicsFamily.value(), 0, &graphicsQueue_);
    vkGetDeviceQueue(device_, indices.presentFamily.value(), 0, &presentQueue_);
}

void Device::createBuffer(
        VkDeviceSize size, 
        VkBufferUsageFlags usage, 
        VkMemoryPropertyFlags properties, 
        VkBuffer* buffer, 
        VkDeviceMemory* bufferMemory)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

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





