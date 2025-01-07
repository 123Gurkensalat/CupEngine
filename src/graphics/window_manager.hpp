#pragma once
#include <memory>
#include <vector>
#include "graphics/instance.hpp"
#include "window.hpp"
#include "device.hpp"
#include "renderer.hpp"

namespace cup 
{
    class WindowManager {
    public: 
        WindowManager(Instance& instance);
        ~WindowManager();

        void run();
        Window& createWindow(const std::string& title);

        Window& mainWindow() { return *windows[0]; }
        Window& currentWindow() { return *currentWindow_; }
    private:
        Instance& instance;

        std::vector<Window*> windows;
        std::unique_ptr<Device> device;
        std::vector<Renderer*> renderers;
        Window* currentWindow_;
    };
}
