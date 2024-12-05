#include "window.hpp"

#include <GLFW/glfw3.h>
#include <string>

using cup::Window;

Window::Window(const std::string& title)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, title.c_str(), nullptr, nullptr);
}

Window::~Window() 
{
    glfwDestroyWindow(window);
    glfwTerminate();
}
