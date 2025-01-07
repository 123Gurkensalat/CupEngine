#include "instance.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

using cup::Instance;

Instance::Instance() 
{
    glfwInit();
    createInstance();
    validator_.setupDebugMessenger(instance_);
}

Instance::~Instance()
{
    validator_.cleanUpDebugMessenger(instance_);
    vkDestroyInstance(instance_, nullptr);
    glfwTerminate();
}

void Instance::createInstance()
{
    if (!validator_.checkValidationLayerSupport()) {
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

    auto extensions = validator_.getRequiredExtensions();
    std::cout << "Used extenstions:\n";
    for(const auto& extension : extensions) {
        std::cout << '\t' << extension << '\n';
    }
    auto debugInfo = validator_.getCreateInfo();

    VkInstanceCreateInfo instanceInfo{};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    instanceInfo.ppEnabledExtensionNames = extensions.data();
    
    if(validator_.enableValidationLayers) {
        instanceInfo.enabledLayerCount = static_cast<uint32_t>(validator_.validationLayers.size());
        instanceInfo.ppEnabledLayerNames = validator_.validationLayers.data();

        instanceInfo.pNext = &debugInfo;
    } else {
        instanceInfo.enabledLayerCount = 0;
    }

    if (vkCreateInstance(&instanceInfo, nullptr, &instance_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}
