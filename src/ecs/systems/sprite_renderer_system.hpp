#pragma once

#include "ecs/components/sprite_renderer.hpp"
#include "ecs/components/transform.hpp"
#include "graphics/pipeline.hpp"
#include "graphics/push_constant_data.hpp"

// std
#include <iostream>
#include <memory>

// vulkan
#include <vulkan/vulkan_core.h>

// ecs
#include <ecs/system.hpp>

namespace cup {
    class SpriteRendererSystem : 
        public ecs::System<SpriteRendererSystem, Transform, SpriteRenderer> {
    public:
        SpriteRendererSystem(
                std::unique_ptr<Pipeline>* pipeline, 
                VkPipelineLayout& pipelineLayout) 
            : pipeline{pipeline}, pipelineLayout{pipelineLayout} 
        {}

        void RenderEntities(VkCommandBuffer& commandBuffer) {
            (*pipeline)->bind(commandBuffer);

            for (auto entity : entities) {
                auto& transform = entity.GetComponent<Transform>();
                auto& renderer = entity.GetComponent<SpriteRenderer>();
                
                PushConstantData push{};
                push.transform = transform.mat();
                push.offset = transform.translation;
                push.color = renderer.color;

                vkCmdPushConstants(
                    commandBuffer, 
                    pipelineLayout, 
                    VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 
                    0,
                    sizeof(PushConstantData),
                    &push);

                renderer.model->bind(commandBuffer);
                renderer.model->draw(commandBuffer);
            }
        }

        std::unique_ptr<Pipeline>* pipeline;
        VkPipelineLayout& pipelineLayout;
    private:
    };
}
