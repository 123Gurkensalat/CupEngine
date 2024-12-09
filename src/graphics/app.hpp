#pragma once

// glfw
#include "graphics/device.hpp"
#include "graphics/pipeline.hpp"
#include "graphics/swap_chain.hpp"
#include "graphics/window.hpp"

// std
#include <string>

namespace cup{
    class App{
    public: 
        App(const std::string &title) : 
            window(title), 
            device(window), 
            swapChain(device, window),
            pipeline(device, "../src/graphics/shader/bin/default.vert.spv", "../src/graphics/shader/bin/default.frag.spv")
        {}

        // starts the application
        void run();

    private:
        Window window;
        Device device;
        SwapChain swapChain;
        Pipeline pipeline;
    };
}
