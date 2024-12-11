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
            pipelineConfig.vertShaderFilePath = "../src/graphics/shader/bin/default.vert.spv"; 
            pipelineConfig.fragShaderFilePath = "../src/graphics/shader/bin/default.frag.spv"; 
            pipeline = std::make_unique<Pipeline>(device, swapChain, pipelineConfig);
        }

        // starts the application
        void run();

    private:
        Window window;
        Device device;
        SwapChain swapChain;
        std::unique_ptr<Pipeline> pipeline;

        VkCommandPool commandPool;
    };
}
