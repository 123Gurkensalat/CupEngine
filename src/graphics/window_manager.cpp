#include "window_manager.hpp"
#include "device.hpp"
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <memory>

using cup::WindowManager;

WindowManager::WindowManager(Instance& instance) : instance(instance) 
{
    windows.push_back(new Window(instance, "Main"));
    device = std::make_unique<Device>(instance, mainWindow());
    renderers.push_back(new Renderer(*device.get(), mainWindow()));

    createWindow("Second");
}

WindowManager::~WindowManager() 
{
    for (Renderer* renderer : renderers) {
        delete renderer;
    }
    for (Window* window : windows) {
        delete window;
    }
}

void WindowManager::run() 
{
    while(!mainWindow().shouldClose()) {
        glfwPollEvents();
        renderers[0]->drawFrame();
    }

    vkDeviceWaitIdle(device->device());
}

cup::Window& WindowManager::createWindow(const std::string& title) 
{
    windows.push_back(new Window(instance, title.c_str()));
    Window& window = *windows.back();

    renderers.push_back(new Renderer(*device.get(), window));

    return window;
}
