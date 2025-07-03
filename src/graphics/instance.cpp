#include "instance.hpp"

#include <GLFW/glfw3.h>
#include <iostream>
#include <vulkan/vulkan.h>

using cup::Instance;

Instance::Instance(const char* app_name, uint32_t app_version)
{
    glfwInit();
    createInstance(app_name, app_version);
}

Instance::~Instance()
{
    validator_.cleanUpDebugMessenger(instance_);
    vkDestroyInstance(instance_, nullptr);
    glfwTerminate();
}

void Instance::createInstance(const char* app_name, uint32_t app_version)
{
    if (!validator_.checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo app_info{
        .pApplicationName   = app_name,
        .applicationVersion = app_version,
        .pEngineName        = "CupEngine",
        .engineVersion      = VK_MAKE_VERSION(0, 1, 0),
        .apiVersion         = VK_API_VERSION_1_3,
    };

    uint32_t available_extensions_count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &available_extensions_count, nullptr);

    std::vector<VkExtensionProperties> available_extensions{available_extensions_count};
    vkEnumerateInstanceExtensionProperties(
        nullptr,
        &available_extensions_count,
        available_extensions.data());

    std::cout << "available instance extensions:\n";
    for (const auto& extension : available_extensions) {
        std::cout << '\t' << extension.extensionName << '\n';
    }

    auto extensions = validator_.getRequiredExtensions();

    std::cout << "Used extenstions:\n";
    for (const auto& extension : extensions) {
        std::cout << '\t' << extension << '\n';
    }

    auto debug_info = validator_.getCreateInfo();

    VkInstanceCreateInfo instance_info{
        .pApplicationInfo        = &app_info,
        .enabledExtensionCount   = static_cast<uint32_t>(extensions.size()),
        .ppEnabledExtensionNames = extensions.data(),
    };

    if (validator_.enableValidationLayers) {
        instance_info.enabledLayerCount = static_cast<uint32_t>(validator_.validationLayers.size());
        instance_info.ppEnabledLayerNames = validator_.validationLayers.data();

        instance_info.pNext = &debug_info;
    } else {
        instance_info.enabledLayerCount = 0;
    }

    if (vkCreateInstance(&instance_info, nullptr, &instance_) != VK_SUCCESS) {
        throw std::runtime_error("Could not create instance");
    }

    validator_.setupDebugMessenger(instance_);
}
