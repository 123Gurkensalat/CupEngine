#pragma once

#include "graphics/validator.hpp"
#include "graphics/window.hpp"
#include <vulkan/vulkan_core.h>
namespace cup 
{
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

        Window& window;
        Validator validator{};

        VkInstance instance;
    };
}
