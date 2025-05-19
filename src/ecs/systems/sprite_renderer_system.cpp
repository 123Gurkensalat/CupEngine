#include "sprite_renderer_system.hpp"
#include "ecs/components/sprite_renderer.hpp"
#include "ecs/components/transform.hpp"
#include "ecs/ecs.hpp"
#include "graphics/camera.hpp"
#include "graphics/render_system.hpp"
#include "resource_manager.hpp"
#include <cassert>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <chrono>
#include <glm/glm.hpp>
#include <cstring>

using cup::SpriteRendererSystem;
using cup::ecs::ECS;

SpriteRendererSystem::SpriteRendererSystem(ECS& ecs, Device& device, VkRenderPass renderPass) : ecs(ecs), device(device), RenderSystem(device) 
{ 
    createLayouts();
    createPipeline(renderPass);
    createDescriptorSets();
}

SpriteRendererSystem::~SpriteRendererSystem()
{
    vkDestroyDescriptorSetLayout(device.device(), descriptorSetLayout, nullptr);
    vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}

void SpriteRendererSystem::createLayouts() 
{
    std::vector<VkDescriptorSetLayoutBinding> bindings(1);

    // sampler layout
    bindings[0].binding = 0;
    bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[0].descriptorCount = 128; // MAX #Textures
    bindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    bindings[0].pImmutableSamplers = nullptr;

    RenderSystem::createDescriptorSetLayout(bindings, &descriptorSetLayout);

    // push constants
    VkPushConstantRange pushConstant{};
    pushConstant.offset = 0;
    pushConstant.size = sizeof(PushConstants);
    pushConstant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

    RenderSystem::createPipelineLayout({descriptorSetLayout}, {pushConstant}, &pipelineLayout);
}

void SpriteRendererSystem::createPipeline(VkRenderPass renderPass) 
{
    PipelineConfigInfo pipelineConfig = Pipeline::defaultPipelineConfigInfo();
    pipelineConfig.vertShaderFilePath = "../src/graphics/shader/bin/default.vert.spv"; 
    pipelineConfig.fragShaderFilePath = "../src/graphics/shader/bin/default.frag.spv"; 
    pipelineConfig.descriptorSetLayout = descriptorSetLayout;
    pipelineConfig.pipelineLayout = pipelineLayout;
    pipelineConfig.renderPass = renderPass;
    RenderSystem::createPipeline(pipelineConfig);
}

void SpriteRendererSystem::createDescriptorSets() 
{
    RenderSystem::createDescriptorSets(descriptorSetLayout, 1, &descriptorSet);
}        

void SpriteRendererSystem::render(VkCommandBuffer commandBuffer, float aspectRatio) 
{
    pipeline->bind(commandBuffer);

    vkCmdBindDescriptorSets(
        commandBuffer, 
        VK_PIPELINE_BIND_POINT_GRAPHICS, 
        pipelineLayout, 
        0, 1, 
        &descriptorSet, 
        0, nullptr);

    static auto startTime = std::chrono::high_resolution_clock::now();
    static auto lastTime = startTime;
    static int counter = 0;
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    counter++;

    // fps counter
    if (std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count() > 1) {
        std::cout << "FPS:" << counter << '\n';
        counter = 0;
        lastTime = currentTime;
    }

    std::function<void(SpriteRenderer&, Transform&)> lambda = [&](SpriteRenderer& spriteRenderer, Transform& transform){
        transform.rotation = time * glm::radians(90.0f);

        PushConstants pushConstants;
        pushConstants.textureIndex = spriteRenderer.spriteIndex();
        auto mvp_mat = Camera::main.worldToCamMat() * transform.mat();
        pushConstants.mvp_mat[0] = glm::vec4(mvp_mat[0], 0.0f);
        pushConstants.mvp_mat[1] = glm::vec4(mvp_mat[1], 0.0f);
        pushConstants.mvp_mat[2] = glm::vec4(mvp_mat[2], 0.0f);

        vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushConstants), &pushConstants);
        
        spriteRenderer.sprite().bind(commandBuffer);
        spriteRenderer.sprite().draw(commandBuffer);
    };

    ecs.forEach(lambda);  
}

void SpriteRendererSystem::setDescriptorSet() const
{
    auto& sprites = ResourceManager::getSprites();
    assert(sprites.size() > 0 && "Default texture not loaded!");

    std::array<VkDescriptorImageInfo, 128> imageInfos;
    for (int i = 0; i < sprites.size(); i++) {
        imageInfos[i] = sprites[i].imageInfo();
    }

    for (int i = sprites.size(); i < 128; i++) {
        imageInfos[i] = sprites[0].imageInfo(); // default sprite
    }

    VkWriteDescriptorSet descriptorWrite{};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = descriptorSet;
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrite.descriptorCount = 128; // MAX #Textures
    descriptorWrite.pImageInfo = imageInfos.data();

    vkUpdateDescriptorSets(
        device.device(), 
        1, 
        &descriptorWrite, 
        0, nullptr);
}
