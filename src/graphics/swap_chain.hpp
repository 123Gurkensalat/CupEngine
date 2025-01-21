#pragma once

#include "device.hpp"
#include "window.hpp"
#include <cstdint>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace cup 
{
    class SwapChain {
    public:
        SwapChain(Device& device, Window& window); 
        ~SwapChain();

        VkResult acquireNextImage(uint32_t* imageIndex) const;
        VkResult submitCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        bool fencesFinished() const;

        VkRenderPass renderPass() const { return renderPass_; }
        const std::vector<VkFramebuffer>& framebuffers() { return framebuffers_; }
        VkExtent2D extent() const { return extent_; }

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

        VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
        VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const;
        VkExtent2D chooseExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
       
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

        size_t currentFrame = 0;
    };
}
