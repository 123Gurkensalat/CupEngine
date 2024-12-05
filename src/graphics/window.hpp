#pragma once

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

        inline bool shouldClose() { return glfwWindowShouldClose(window); }

        static constexpr uint32_t WIDTH = 800; 
        static constexpr uint32_t HEIGHT = 600; 
    private:
        GLFWwindow* window;
    };
}
