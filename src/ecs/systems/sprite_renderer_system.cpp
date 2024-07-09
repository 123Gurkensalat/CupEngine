#include "sprite_renderer_system.hpp"
#include "graphics/push_constant_data.hpp"
#include <stdexcept>
#include <vulkan/vulkan_core.h>

namespace cup {
    SpriteRendererSystem::SpriteRendererSystem(Device& device, VkRenderPass renderPass)
        : device{device}
    {
        createPipelineLayout();
        createPipeline(renderPass);
    }

    
    SpriteRendererSystem::~SpriteRendererSystem() {
        vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
    }

    void SpriteRendererSystem::createPipelineLayout(){
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(PushConstantData);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

        if(vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
             throw std::runtime_error("failed to create pipeline layout");
        }
    }    

    void SpriteRendererSystem::createPipeline(VkRenderPass renderPass) {
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipelineLayout");

        PipelineConfigInfo pipelineConfig{}; 
        Pipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        pipeline = std::make_unique<Pipeline>(
            device,
            "../src/graphics/shader/bin/default_shader.vert.spv",
            "../src/graphics/shader/bin/default_shader.frag.spv",
            pipelineConfig
        );
    }

    void SpriteRendererSystem::renderEntities(VkCommandBuffer commandBuffer) {
            pipeline->bind(commandBuffer);

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
}
