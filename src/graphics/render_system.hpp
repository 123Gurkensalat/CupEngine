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

        void render(VkCommandBuffer commandBuffer, size_t currentFrame, float aspectRatio);

    private:
        struct UniformBufferObject {
            glm::mat4 model;
            glm::mat4 view;
            glm::mat4 proj;
        };

        void createDescriptorSetLayout();
        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);
        
        void createDescriptorPool();
        void createDescriptorSets();
        void createUniformBuffers(); 

        Device& device;

        VkDescriptorSetLayout descriptorSetLayout;
        VkPipelineLayout pipelineLayout;
        std::unique_ptr<Pipeline> pipeline;
        std::shared_ptr<Model> model;

        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;
        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        std::vector<void*> uniformBuffersMapped;
    };
}
