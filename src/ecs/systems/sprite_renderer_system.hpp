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

        void setDescriptorSet() const;
    private:
        struct alignas(16) PushConstants{
            glm::vec4 mvp_mat[3]; // mat3 with right alignement
            uint32_t textureIndex;
        };
        
        void createLayouts();
        void createPipeline(VkRenderPass);

        void createDescriptorSets();
        void createPushConstants();

        ecs::ECS& ecs;
        Device& device;

        // pipeline infos
        VkDescriptorSetLayout descriptorSetLayout;
        VkPipelineLayout pipelineLayout;
        VkDescriptorSet descriptorSet;
    };
}
