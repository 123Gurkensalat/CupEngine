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
    private:
        void init();
        void createSwapChain();
        void createImageViews();

        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
       
        Device& device;
        Window& window;

        VkSwapchainKHR swapChain;
        std::vector<VkImage> images;
        std::vector<VkImageView> imageViews;

        VkFormat imageFormat_;
        VkExtent2D extent_;

    };
}
