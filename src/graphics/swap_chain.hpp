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

        VkRenderPass getRenderPass() { return renderPass; }

    private:
        void init();
        void createSwapChain();
        void createImageViews();
        void createRenderPass();

        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
       
        Device& device;
        Window& window;

        VkSwapchainKHR swapChain;
        std::vector<VkImage> images;
        std::vector<VkImageView> imageViews;
        VkRenderPass renderPass;

        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;

    };
}
