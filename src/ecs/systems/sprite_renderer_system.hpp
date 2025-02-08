#pragma once

#include "ecs/ecs.hpp"
#include "graphics/device.hpp"
#include "graphics/pipeline.hpp"
#include <glm/glm.hpp>
#include <vulkan/vulkan_core.h>

namespace cup {
    class SpriteRendererSystem {
    public:
        SpriteRendererSystem(ecs::ECS& ecs, Device& device) : ecs{ecs}, device{device} {} 
        void render();
    private:
        struct UniformBufferObject {
            glm::mat4 model;
            glm::mat4 view;
            glm::mat4 proj;
        };
        
        void createDescriptorSetLayout();
        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);

        void createDescriptorSets();
        void createUniformBuffers(); 

        ecs::ECS& ecs;
        Device& device;

        // pipeline infos
        VkDescriptorSetLayout descriptorSetLayout;
        VkPipelineLayout pipelineLayout;
        std::unique_ptr<Pipeline> pipeline;

        // uniform buffers
        std::vector<VkDescriptorSet> descriptorSets;
        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        std::vector<void*> uniformBuffersMapped;


    };
}
