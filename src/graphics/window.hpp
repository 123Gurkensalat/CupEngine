#pragma once

#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace cup 
{
    class Window {
    public:
        Window(const std::string& title);
        ~Window();
        
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        void createSurface(VkInstance instance, VkSurfaceKHR* surface);

        inline bool shouldClose() { return glfwWindowShouldClose(window); }
        
        // extent in pixel
        VkExtent2D getExtent();

        static constexpr uint32_t WIDTH = 800; 
        static constexpr uint32_t HEIGHT = 600; 
    private:
        GLFWwindow* window;
    };
}
