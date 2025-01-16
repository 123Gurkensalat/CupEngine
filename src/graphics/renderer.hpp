#pragma once

#include "device.hpp"
#include "graphics/model.hpp"
#include "pipeline.hpp"
#include "swap_chain.hpp"
#include "window.hpp"
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace cup 
{
    class Renderer {
    public:
        Renderer(Device& device, Window& window);
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        Renderer(Renderer&&) = delete;
        Renderer& operator=(Renderer&&) = delete;

        VkCommandBuffer beginTransferCommands();
        void endTransferCommands(VkCommandBuffer);
        void drawFrame();
        bool finished();

    private:
        void createPipeline();
        void createCommandPool(uint32_t queueFamilyIndex, VkCommandPool* commandPool);
        void createCommandBuffer();
        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        Device& device;
        Window& window;
        SwapChain swapChain{device, window};
        std::unique_ptr<Pipeline> pipeline;

        VkCommandPool graphicsCommandPool;
        VkCommandPool transferCommandPool;
        std::vector<VkCommandBuffer> commandBuffers;
        uint32_t currentFrame = 0; 

        // temp
        std::unique_ptr<Model> model;
    };
}
