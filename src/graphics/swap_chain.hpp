#pragma once

#include "graphics/device.hpp"
#include "graphics/window.hpp"
#include <vector>
#include <vulkan/vulkan_core.h>
namespace cup 
{
    class SwapChain {
    public:
        SwapChain(Device& device, Window& window); 
        ~SwapChain();

        VkResult acquireNextImage(uint32_t* imageIndex);
        VkResult submitCommandBuffer(const VkCommandBuffer commandBuffer, const uint32_t imageIndex);

        const VkRenderPass getRenderPass() { return renderPass; }
        const std::vector<VkFramebuffer>& getFrameBuffers() { return framebuffers; }
        const VkExtent2D getExtent() { return swapChainExtent; }

    private:
        void init();
        void createSwapChain();
        void createImageViews();
        void createRenderPass();
        void createFramebuffers();
        void createSyncObjects();

        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
       
        Device& device;
        Window& window;

        VkSwapchainKHR swapChain;
        std::vector<VkImage> images;
        std::vector<VkImageView> imageViews;
        VkRenderPass renderPass;
        std::vector<VkFramebuffer> framebuffers;
        
        VkSemaphore imageAvailableSemaphore;
        VkSemaphore renderFinishedSemaphore;
        VkFence inFlightFence;

        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;

    };
}
