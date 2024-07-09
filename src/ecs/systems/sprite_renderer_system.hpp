#pragma once

#include "ecs/components/sprite_renderer.hpp"
#include "ecs/components/transform.hpp"
#include "graphics/device.hpp"
#include "graphics/pipeline.hpp"

// std
#include <memory>

// vulkan
#include <vulkan/vulkan_core.h>

// ecs
#include <ecs/system.hpp>

namespace cup {
    class SpriteRendererSystem : 
        public ecs::System<SpriteRendererSystem, Transform, SpriteRenderer> {
    public:
        SpriteRendererSystem(Device& device, VkRenderPass renderPass);
        ~SpriteRendererSystem();

        SpriteRendererSystem(const SpriteRendererSystem&) = delete;
        SpriteRendererSystem& operator=(const SpriteRendererSystem&) = delete;

        void renderEntities(VkCommandBuffer commandBuffer);

    private:
        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);

        Device& device;

        std::unique_ptr<Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;
    };
}
