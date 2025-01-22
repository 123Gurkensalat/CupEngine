#pragma once

#include "device.hpp"
#include "render_system.hpp"
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

        const SwapChain& swapChain() const { return swapChain_; }

        void draw();

        VkCommandBuffer beginTransferCommands();
        void endTransferCommands(VkCommandBuffer);

        bool finished() const { return swapChain_.fencesFinished(); }

    private:
        void createCommandPool(uint32_t queueFamilyIndex, VkCommandPool* commandPool);
        void createCommandBuffer();
        void createTextureImage();
        void createTextureImageView();
        void createTextureSampler();

        VkCommandBuffer beginFrame();
        void endFrame();

        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void copyBufferToImage(VkBuffer buffer, VkImage image, VkExtent3D extent);

        Device& device;
        Window& window;

        SwapChain swapChain_{device, window};

        VkCommandPool graphicsCommandPool;
        VkCommandPool transferCommandPool;
        std::vector<VkCommandBuffer> commandBuffers; 
        
        std::unique_ptr<RenderSystem> renderSystem;

        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        VkImageView textureImageView;
        VkSampler textureSampler;

        uint32_t currentImageIndex = 0; 
        uint32_t currentFrame = 0;
    };
}
