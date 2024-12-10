#pragma once

// glfw
#include "graphics/device.hpp"
#include "graphics/pipeline.hpp"
#include "graphics/swap_chain.hpp"
#include "graphics/window.hpp"

// std
#include <memory>
#include <string>

namespace cup{
    class App{
    public: 
        App(const std::string &title) : 
            window(title), 
            device(window), 
            swapChain(device, window)
        {
            PipelineConfigInfo pipelineConfig{};
            Pipeline::defaultPipelineConfigInfo(pipelineConfig);
            pipeline = std::make_unique<Pipeline>(
                    device, swapChain, 
                    "../src/graphics/shader/bin/default.vert.spv", 
                    "../src/graphics/shader/bin/default.frag.spv", 
                    pipelineConfig);
        }

        // starts the application
        void run();

    private:
        Window window;
        Device device;
        SwapChain swapChain;
        std::unique_ptr<Pipeline> pipeline;
    };
}
