#pragma once

#include "device.hpp"
#include "ecs/ecs.hpp"
#include "ecs/systems/sprite_renderer_system.hpp"
#include "swap_chain.hpp"
#include "window.hpp"
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace cup 
{
    class Renderer {
    public:
        Renderer(ecs::ECS& ecs, Device& device, Window& window);
        ~Renderer() = default;

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        Renderer& operator=(Renderer&&) = delete;

        const SwapChain& swapChain() const { return swapChain_; }

        void draw();

        bool finished() const { return swapChain_.fencesFinished(); }

    private:
        void createCommandPool(uint32_t queueFamilyIndex, VkCommandPool* commandPool);
        void createCommandBuffer();

        VkCommandBuffer beginFrame();
        void endFrame();

        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

        Device& device;
        Window& window;

        SwapChain swapChain_{device, window};

        std::vector<VkCommandBuffer> commandBuffers; 
        
        std::unique_ptr<SpriteRendererSystem> spriteRenderSystem;

        uint32_t currentImageIndex = 0; 
        uint32_t currentFrame = 0;
    };
}
