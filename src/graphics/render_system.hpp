#pragma once

#include "device.hpp"
#include "graphics/sprite.hpp"
#include "pipeline.hpp"
#include <memory>
#include <vulkan/vulkan_core.h>
#include <glm/mat4x4.hpp>

namespace cup 
{
    class Renderer;
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
        
        void createDescriptorSets();
        void createUniformBuffers(); 

        void updateDescriptorSets(size_t currentFrame, VkDescriptorImageInfo& imageInfo) const;

        Device& device;
        Renderer& renderer;

        VkDescriptorSetLayout descriptorSetLayout;
        VkPipelineLayout pipelineLayout;
        std::unique_ptr<Pipeline> pipeline;
        std::shared_ptr<Sprite> model;

        std::vector<VkDescriptorSet> descriptorSets;
        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        std::vector<void*> uniformBuffersMapped;
    };
}
