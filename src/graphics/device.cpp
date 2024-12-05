#include "device.hpp"
#include "graphics/window.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

using cup::Device;

Device::Device(cup::Window& window) : window(window) {
    createVkInstance();
    validator.setupDebugMessenger(instance);
}

Device::~Device() {
    validator.cleanUpDebugMessenger(instance);
    vkDestroyInstance(instance, nullptr);
}

void Device::createVkInstance() 
{
    if (!validator.checkValidationLayerSupport()) 
        throw std::runtime_error("validation layers requested, but not available!");

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "ApplicationName";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "CupEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    uint32_t availableExtensionsCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionsCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(availableExtensionsCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionsCount, availableExtensions.data());

    std::cout << "available extensions:\n";
    for(const auto& extension : availableExtensions) 
    {
        std::cout << '\t' << extension.extensionName << '\n';
    }

    auto extensions = validator.getRequiredExtensions();

    auto debugCreateInfo = validator.getDebugMessengerCreateInfo();
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    if(validator.enableValidationLayers) 
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validator.validationLayers.size());
        createInfo.ppEnabledLayerNames = validator.validationLayers.data();

        createInfo.pNext = &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to create instance!");
    }
}
