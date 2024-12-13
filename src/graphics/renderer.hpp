#pragma once

#include "graphics/device.hpp"
#include "graphics/pipeline.hpp"
#include "graphics/swap_chain.hpp"
#include "graphics/window.hpp"
#include <memory>
#include <vulkan/vulkan_core.h>
namespace cup 
{
    class Renderer {
    public:
        Renderer(Device& device, Window& window);
        ~Renderer();

        void drawFrame();

    private:
        void createPipeline();
        void createCommandPool();
        void createCommandBuffer();
        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        Device& device;
        Window& window;
        SwapChain swapChain{device, window};
        std::unique_ptr<Pipeline> pipeline;

        VkCommandPool commandPool;
        VkCommandBuffer commandBuffer;
    };
}
