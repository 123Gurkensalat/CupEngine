#pragma once

// glfw
#include "graphics/device.hpp"
#include "graphics/renderer.hpp"
#include "graphics/window.hpp"

// std
#include <string>

namespace cup{
    class App{
    public: 
        App();

        // starts the application
        void run();

    private:
        Window window{"CupEngine"};
        Device device{window};
        Renderer renderer{device, window};
    };
}
