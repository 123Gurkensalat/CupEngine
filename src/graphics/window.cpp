#include "window.hpp"

#include <GLFW/glfw3.h>
#include <stdexcept>
#include <string>

using cup::Window;

Window::Window(const std::string& title)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, title.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

Window::~Window() 
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::createSurface(VkInstance instance, VkSurfaceKHR* surface)
{
    if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}

VkExtent2D Window::getExtent() 
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    return {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    };
}

void Window::framebufferResizeCallback(GLFWwindow *glfwWindow, int width, int height)
{
    auto window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
    window->framebufferResized = true;
}
