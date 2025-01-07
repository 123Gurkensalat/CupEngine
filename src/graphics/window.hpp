#pragma once

#include "graphics/instance.hpp"
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace cup 
{
    class Window {
    public:
        Window(Instance& instance, const char* title);
        ~Window();
        
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;

        inline bool shouldClose() { return glfwWindowShouldClose(window_); }
        inline bool wasResized() { return framebufferResized; }
        inline void resetResizedFlag() { framebufferResized = false; }
        
        // extent in pixel
        VkExtent2D extent();
        VkSurfaceKHR surface() const { return surface_; }

        static constexpr uint32_t WIDTH = 800; 
        static constexpr uint32_t HEIGHT = 600; 

    private:
        void createWindow(const char* title);
        void createSurface();
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

        Instance& instance;
        GLFWwindow* window_;
        VkSurfaceKHR surface_;
        bool framebufferResized = false;
    };
}
