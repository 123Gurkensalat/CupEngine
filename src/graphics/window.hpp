#pragma once

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
        inline bool wasResized() { return framebufferResized; }
        inline void resetResizedFlag() { framebufferResized = false; }
        
        // extent in pixel
        VkExtent2D getExtent();

        static constexpr uint32_t WIDTH = 800; 
        static constexpr uint32_t HEIGHT = 600; 

    private:
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

        GLFWwindow* window;
        bool framebufferResized = false;
    };
}
