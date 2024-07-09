#pragma once

#include "ecs/systems/sprite_renderer_system.hpp"
#include "window.hpp"
#include "swap_chain.hpp"

//std
#include <cassert>
#include <cstdint>
#include <memory>
#include <vector>

// vulkan
#include <vulkan/vulkan_core.h>

namespace cup {
    class Renderer {
    public:
        Renderer(Window& window, Device& device);
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer operator=(const Renderer&) = delete;

        VkRenderPass getSwapChainRenderPass() const { return swapChain->getRenderPass(); }

        bool isFrameInProgress() const { return isFrameStarted; }

        VkCommandBuffer getCurrentCommandBuffer() const {
            assert(isFrameStarted && "Cannot get commandBuffers before starting a frame");
            return commandBuffers[currentFrameIndex];
        }

        int getFrameIndex() const { 
            assert(isFrameStarted && "Cannot get frame index when frame is not in progress");
            return currentFrameIndex;
        }

        VkCommandBuffer beginFrame();
        void endFrame();

        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
    private:
        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();

        Window& window;
        Device& device;
        std::unique_ptr<SwapChain> swapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex{};
        int currentFrameIndex{};
        bool isFrameStarted{};
    };
}
