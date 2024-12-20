#include "app.hpp"
#include "graphics/window_manager.hpp"
#include <vulkan/vulkan_core.h>

// glfw  + vulkan
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

using cup::App;

void App::run() 
{
    Window& window = windowManager.mainWindow();

    while(!window.shouldClose()) {
        glfwPollEvents();
        renderer.drawFrame();
    }

    vkDeviceWaitIdle(device.device());
}
