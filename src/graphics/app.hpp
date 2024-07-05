#pragma once

#include "ecs/systems/sprite_renderer_system.hpp"
#include "pipeline.hpp"
#include "window.hpp"
#include "swap_chain.hpp"

//std
#include <memory>
#include <vector>

// vulkan
#include <vulkan/vulkan_core.h>

namespace cup {
    class App {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;
        
        App();
        ~App();

        App(const App&) = delete;
        App operator=(const App&) = delete;

        void run();
    private:
        void loadEntities();
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void freeCommandBuffers();
        void drawFrame();
        void recreateSwapChain();
        void recordCommandBuffer(int imageIndex);
        void renderGameObjects(); // possibly convert this to system

        Window window{WIDTH, HEIGHT, "CupEngine"};
        Device device{window};
        std::unique_ptr<SwapChain> swapChain;
        std::unique_ptr<Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::shared_ptr<SpriteRendererSystem> spriteRenderer;
    };
}
