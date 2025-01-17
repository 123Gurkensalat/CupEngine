#pragma once

#include "graphics/device.hpp"
#include "graphics/window.hpp"
#include <cstdint>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace cup 
{
    class SwapChain {
    public:
        SwapChain(Device& device, Window& window); 
        ~SwapChain();

        VkResult acquireNextImage(uint32_t currentFrame, uint32_t* imageIndex);
        VkResult submitCommandBuffer(uint32_t currentFrame, VkCommandBuffer commandBuffer, uint32_t imageIndex);
        bool fencesFinished();

        const VkRenderPass renderPass() { return renderPass_; }
        const std::vector<VkFramebuffer>& framebuffers() { return framebuffers_; }
        const VkExtent2D extent() const { return extent_; }

        void recreateSwapChain();

        static constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;

    private:
        void init();
        void createSwapChain();
        void createImageViews();
        void createRenderPass();
        void createFramebuffers();
        void createSyncObjects();
        void cleanupSwapChain();

        VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseExtent(const VkSurfaceCapabilitiesKHR& capabilities);
       
        Device& device;
        Window& window;

        VkSwapchainKHR swapChain;
        std::vector<VkImage> images;
        std::vector<VkImageView> imageViews;

        VkRenderPass renderPass_;
        std::vector<VkFramebuffer> framebuffers_;

        VkFormat imageFormat_;
        VkExtent2D extent_;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
    };
}
