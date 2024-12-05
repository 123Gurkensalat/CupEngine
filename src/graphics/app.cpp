#include "app.hpp"

// glfw  + vulkan
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

using cup::App;

void App::run() 
{
    while(!window.shouldClose()) 
    {
        glfwPollEvents();
    }
}
