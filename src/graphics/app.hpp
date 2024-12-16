#pragma once

// glfw
#include "device.hpp"
#include "renderer.hpp"
#include "window.hpp"

// std
#include <string>

namespace cup{
    class App{
    public: 
        App() = default;
        App(const App&) = delete;
        App& operator=(const App&) = delete;

        /// starts the application
        void run();

    private:
        Window window{"CupEngine"};
        Device device{window};
        Renderer renderer{device, window};
    };
}
