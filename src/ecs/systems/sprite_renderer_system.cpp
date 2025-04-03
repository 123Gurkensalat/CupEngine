#include "sprite_renderer_system.hpp"
#include "ecs/components/sprite_renderer.hpp"
#include "ecs/components/transform.hpp"
#include "ecs/ecs.hpp"
#include "graphics/camera.hpp"
#include "graphics/render_system.hpp"
#include "graphics/swap_chain.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
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
    RenderSystem::createUniformBuffers<UniformBufferObject>(uniformBuffers, uniformBuffersMemory, uniformBuffersMapped);
    createDescriptorSets();
}

SpriteRendererSystem::~SpriteRendererSystem()
{
    for (size_t i = 0; i < SwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroyBuffer(device.device(), uniformBuffers[i], nullptr);
        vkFreeMemory(device.device(), uniformBuffersMemory[i], nullptr);
    }

    vkDestroyDescriptorSetLayout(device.device(), descriptorSetLayout, nullptr);
    vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}

void SpriteRendererSystem::createLayouts() 
{
    std::vector<VkDescriptorSetLayoutBinding> bindings(2);

    // ubo layout
    bindings[0].binding = 0;
    bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[0].descriptorCount = 1;
    bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    bindings[0].pImmutableSamplers = nullptr;

    // sampler layout
    bindings[1].binding = 1;
    bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[1].descriptorCount = 1;
    bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    bindings[1].pImmutableSamplers = nullptr;

    RenderSystem::createDescriptorSetLayout(bindings, &descriptorSetLayout);

    RenderSystem::createPipelineLayout({descriptorSetLayout}, {}, &pipelineLayout);
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
    RenderSystem::createDescriptorSets(descriptorSetLayout, descriptorSets);

    for (size_t i = 0; i < SwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        std::array<VkWriteDescriptorSet, 1> descriptorWrites{};
        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = descriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(
            device.device(), 
            static_cast<uint32_t>(descriptorWrites.size()), 
            descriptorWrites.data(), 
            0, nullptr);
    }
}        

void SpriteRendererSystem::render(VkCommandBuffer commandBuffer, float aspectRatio) 
{
    pipeline->bind(commandBuffer);

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

        UniformBufferObject ubo;
        transform.rotation = time * glm::radians(90.0f);
        ubo.model = transform.mat();
        ubo.view = Camera::main.worldToCamMat();
        ubo.proj = glm::mat4({1.0f, -1.0f, 1.0f, 1.0f});

        memcpy(uniformBuffersMapped[currentFrame], &ubo, sizeof(ubo));

        VkDescriptorImageInfo imageInfo = spriteRenderer.sprite().imageInfo();
        updateDescriptorSets(currentFrame, imageInfo);

        vkCmdBindDescriptorSets(
                commandBuffer, 
                VK_PIPELINE_BIND_POINT_GRAPHICS, 
                pipelineLayout, 
                0, 1, 
                &descriptorSets[currentFrame], 
                0, nullptr);

        spriteRenderer.sprite().bind(commandBuffer);
        spriteRenderer.sprite().draw(commandBuffer);
    };

    ecs.forEach(lambda);  
    currentFrame = (currentFrame + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
}

void SpriteRendererSystem::updateDescriptorSets(size_t currentFrame, VkDescriptorImageInfo& imageInfo) const
{
    VkWriteDescriptorSet descriptorWrite{};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = descriptorSets[currentFrame];
    descriptorWrite.dstBinding = 1;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pImageInfo = &imageInfo;

    vkUpdateDescriptorSets(
        device.device(), 
        1, 
        &descriptorWrite, 
        0, nullptr);
}
