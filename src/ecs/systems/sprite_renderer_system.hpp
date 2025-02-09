#pragma once

#include "ecs/ecs.hpp"
#include "graphics/device.hpp"
#include "graphics/render_system.hpp"
#include <glm/glm.hpp>
#include <vulkan/vulkan_core.h>

namespace cup {
    class SpriteRendererSystem : protected RenderSystem {
    public:
        SpriteRendererSystem(ecs::ECS& ecs, Device& device, VkRenderPass renderPass);
        ~SpriteRendererSystem();
        void render(VkCommandBuffer commandBuffer, float ascpectRatio);

    private:
        struct UniformBufferObject {
            glm::mat4 model;
            glm::mat4 view;
            glm::mat4 proj;
        };
        
        void createLayouts();
        void createPipeline(VkRenderPass);

        void createDescriptorSets();

        void updateDescriptorSets(size_t currentFrame, VkDescriptorImageInfo& imageInfo) const;

        ecs::ECS& ecs;
        Device& device;

        // pipeline infos
        VkDescriptorSetLayout descriptorSetLayout;
        VkPipelineLayout pipelineLayout;

        // uniform buffers
        std::vector<VkDescriptorSet> descriptorSets;
        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        std::vector<void*> uniformBuffersMapped;

        uint32_t currentFrame = 0;
    };
}
