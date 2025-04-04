#pragma once

#include "graphics/instance.hpp"
#include <tuple>
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace cup 
{
    namespace input { class InputManager; }
    class Window {
    public:
        enum Status {
            ACTIVE,
            CLOSING,
            CLOSED
        };
        typedef std::tuple<Window*, input::InputManager*> UserTuple;

        Window(Instance& instance, const char* title);
        ~Window();
        
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;

        bool shouldClose() const { return glfwWindowShouldClose(window_); }
        bool wasResized() const { return framebufferResized; }
        void resetResizedFlag() { framebufferResized = false; }
        
        // extent in pixel
        VkExtent2D extent() const;
        VkSurfaceKHR surface() const { return surface_; }
        GLFWwindow* window() const { return window_; }
        
        template<typename T>
        void setUserTupleElement(T* e);
        template<typename T>
        static void setUserTupleElement(GLFWwindow* window, T* e);
        template<typename T>
        T& getUserTupleElement();
        template<typename T>
        static T& getUserTupleElement(GLFWwindow* window);

        static constexpr uint32_t WIDTH = 800; 
        static constexpr uint32_t HEIGHT = 600; 

        Status status = ACTIVE;
    private:
        void createWindow(const char* title);
        void createSurface();
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

        Instance& instance;
        GLFWwindow* window_;
        VkSurfaceKHR surface_;
        bool framebufferResized = false;
    };

    template<typename T>
    void Window::setUserTupleElement(T* e)
    {
        setUserTupleElement(window_, e);
    }

    template<typename T>
    void Window::setUserTupleElement(GLFWwindow* window, T* e)
    {
        if (!glfwGetWindowUserPointer(window)) {
            auto tuple = new UserTuple{};
            std::get<T*>(*tuple) = e;
            glfwSetWindowUserPointer(window, tuple);
        } else {
            auto tuple = reinterpret_cast<UserTuple*>(glfwGetWindowUserPointer(window));
            std::get<T*>(*tuple) = e;
            glfwSetWindowUserPointer(window, tuple);
        }
    }

    template<typename T>
    T& Window::getUserTupleElement() 
    {
        return getUserTupleElement<T>(window_);
    }

    template<typename T>
    T& Window::getUserTupleElement(GLFWwindow* window) 
    {
        auto tuple = reinterpret_cast<UserTuple*>(glfwGetWindowUserPointer(window));
        return *std::get<T*>(*tuple);
    }

}
