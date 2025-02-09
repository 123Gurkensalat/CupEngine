#pragma once
#include <memory>
#include <vector>
#include "ecs/ecs.hpp"
#include "graphics/instance.hpp"
#include "window.hpp"
#include "device.hpp"
#include "renderer.hpp"

namespace cup 
{
    class WindowManager {
    public: 
        WindowManager(ecs::ECS& ecs, Instance& instance);
        ~WindowManager();

        void run();
        Window& createWindow(const std::string& title);

        const Window& mainWindow() const { return *windows[0]; }
        const Window& currentWindow() const { return *currentWindow_; }

        Device& getDevice() const { return *device; }
    private:
        void createMainWindow();

        ecs::ECS& ecs;
        Instance& instance;

        std::vector<Window*> windows;
        std::unique_ptr<Device> device;
        std::vector<Renderer*> renderers;
        Window* currentWindow_;
        uint32_t windowsCount = 0;
    };
}
