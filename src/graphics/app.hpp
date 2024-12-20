#pragma once

// glfw
#include "device.hpp"
#include "window_manager.hpp"
#include "renderer.hpp"

namespace cup{
    class App{
    public: 
        App() = default;
        App(const App&) = delete;
        App& operator=(const App&) = delete;

        /// starts the application
        void run();

    private:
        WindowManager windowManager{};
        Device device{windowManager.mainWindow()};
        Renderer renderer{device, windowManager.mainWindow()};
    };
}
