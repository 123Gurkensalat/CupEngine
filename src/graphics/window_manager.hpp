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

        const Window& mainWindow() const { return *windows[0]; }
        const Window& currentWindow() const { return *currentWindow_; }
    private:
        void createMainWindow();

        Instance& instance;

        std::vector<Window*> windows;
        std::unique_ptr<Device> device;
        std::vector<Renderer*> renderers;
        Window* currentWindow_;
        uint32_t windowsCount = 0;
    };
}
