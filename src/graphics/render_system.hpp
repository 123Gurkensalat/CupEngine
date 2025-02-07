#pragma once

#include "device.hpp"
#include "model.hpp"
#include "pipeline.hpp"
#include <memory>
#include <vulkan/vulkan_core.h>

namespace cup 
{
    class RenderSystem {
    public:
        RenderSystem(Device& device, Renderer& renderer);
        ~RenderSystem();

        void render(VkCommandBuffer commandBuffer, size_t currentFrame, float aspectRatio) const;

    private:
        struct UniformBufferObject {
            glm::mat4 model;
            glm::mat4 view;
            glm::mat4 proj;
        };
        
        // move pool creation to device or resource manager, helper functions somewhere else too
        void createDescriptorSetLayout();
        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);
        
        void createDescriptorPool();
        void createDescriptorSets();
        void createUniformBuffers(); 

        void createTextureImage();
        void createTextureImageView();
        void createTextureSampler();

        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void copyBufferToImage(VkBuffer buffer, VkImage image, VkExtent3D extent);

        Device& device;
        Renderer& renderer;

        VkDescriptorSetLayout descriptorSetLayout;
        VkPipelineLayout pipelineLayout;
        std::unique_ptr<Pipeline> pipeline;
        std::shared_ptr<Model> model;

        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;
        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        std::vector<void*> uniformBuffersMapped;

        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        VkImageView textureImageView;
        VkSampler textureSampler;
    };
}
