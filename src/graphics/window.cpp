#include "window.hpp"

#include <cassert>
#include <stdexcept>

using cup::Window;

Window::Window(Instance& instance, const char* title) : instance(instance)
{
    createWindow(title);
    createSurface();
}

Window::~Window() 
{
    vkDestroySurfaceKHR(instance, surface_, nullptr);
    glfwDestroyWindow(window_);
}

void Window::createWindow(const char* title)
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window_ = glfwCreateWindow(WIDTH, HEIGHT, title, nullptr, nullptr);
    glfwSetWindowUserPointer(window_, this);
    glfwSetFramebufferSizeCallback(window_, framebufferResizeCallback);
}

void Window::createSurface()
{
    if (glfwCreateWindowSurface(instance, window_, nullptr, &surface_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}

VkExtent2D Window::extent() const 
{
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
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
