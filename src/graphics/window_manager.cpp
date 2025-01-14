#include "window_manager.hpp"
#include "device.hpp"
#include "graphics/window.hpp"
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

using namespace cup;

WindowManager::WindowManager(Instance& instance) : instance(instance) 
{
    createMainWindow(); 
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

        // update windows
        for (int i = 0; i < windowsCount; i++) {
            if (renderers[i] == nullptr) continue;
            if (windows[i]->status == Window::CLOSED) continue;
            renderers[i]->drawFrame();
            if (windows[i]->status == Window::CLOSING) {
                windows[i]->status = Window::CLOSED;
            }
        }

        // check for windows closing events
        for (uint32_t i = 1; i < windows.size(); i++) {
            if (windows[i] == nullptr) continue;
            if (!windows[i]->shouldClose()) continue;
            if (windows[i]->status == Window::ACTIVE) {
                windows[i]->status = Window::CLOSING;
            }

            if (!renderers[i]->finished()) continue;
            delete renderers[i];
            delete windows[i];
            renderers[i] = nullptr;
            windows[i] = nullptr;
            windowsCount -= 1;
        }
    }

    vkDeviceWaitIdle(device->device());
}

void WindowManager::createMainWindow()
{
    windows.push_back(new Window(instance, "Main"));
    device = std::make_unique<Device>(instance, mainWindow());
    renderers.push_back(new Renderer(*device.get(), mainWindow()));
    windowsCount += 1;
}

Window& WindowManager::createWindow(const std::string& title) 
{
    // add new window to back if vector is full
    if (windowsCount == windows.size()) 
    {
        windows.push_back(new Window(instance, title.c_str()));
        Window& window = *windows.back();

        renderers.push_back(new Renderer(*device.get(), window));
        windowsCount += 1;
        return window;
    }

    // add new window in between when vector is not full
    for (uint32_t i = 1; i < windows.size(); i++) {
        if (windows[i] != nullptr) continue;

        // if its empty create new window in its place
        windows[i] = new Window(instance, title.c_str());
        renderers[i] = new Renderer(*device.get(), *windows[i]);
        windowsCount += 1;
        return *windows[i];
    }

    throw std::runtime_error("failed to create new window. Window count is out of sync.");
}
