#include "app.hpp"
#include <vulkan/vulkan_core.h>

// glfw  + vulkan
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

using cup::App;

App::App()
{
}

void App::run() 
{
    while(!window.shouldClose()) 
    {
        glfwPollEvents();
        renderer.drawFrame();
    }

    vkDeviceWaitIdle(device.device());
}
