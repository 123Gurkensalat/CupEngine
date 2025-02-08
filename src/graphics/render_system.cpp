#include "render_system.hpp"
#include "graphics/sprite.hpp"
#include "swap_chain.hpp"
#include "renderer.hpp"
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vulkan/vulkan_core.h>
#include <chrono>
#include <cstring>
#include <stb_image.h>

using namespace cup;

RenderSystem::RenderSystem(Device& device, Renderer& renderer) : device(device), renderer(renderer) 
{
    createDescriptorSetLayout();
    createPipelineLayout();
    createPipeline(renderer.swapChain().renderPass());
    createUniformBuffers();

    model = std::make_shared<Sprite>(device);
    
    createDescriptorSets();

}

RenderSystem::~RenderSystem() 
{
    for (size_t i = 0; i < SwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroyBuffer(device.device(), uniformBuffers[i], nullptr);
        vkFreeMemory(device.device(), uniformBuffersMemory[i], nullptr);
    }

    vkDestroyDescriptorSetLayout(device.device(), descriptorSetLayout, nullptr);
    vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}

void RenderSystem::createDescriptorSetLayout() 
{
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    samplerLayoutBinding.pImmutableSamplers = nullptr;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(device.device(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

void RenderSystem::createPipelineLayout() 
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = VK_NULL_HANDLE;

    if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("could not create pipeline layout!");
    }
}

void RenderSystem::createPipeline(VkRenderPass renderPass) 
{
    PipelineConfigInfo pipelineConfig{};
    Pipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.vertShaderFilePath = "../src/graphics/shader/bin/default.vert.spv"; 
    pipelineConfig.fragShaderFilePath = "../src/graphics/shader/bin/default.frag.spv"; 
    pipelineConfig.descriptorSetLayout = descriptorSetLayout;
    pipelineConfig.pipelineLayout = pipelineLayout;
    pipelineConfig.renderPass = renderPass;
    pipeline = std::make_unique<Pipeline>(device, pipelineConfig);
}

void RenderSystem::createDescriptorSets() 
{
    std::vector<VkDescriptorSetLayout> layouts{SwapChain::MAX_FRAMES_IN_FLIGHT, descriptorSetLayout};
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = device.descriptorPool();
    allocInfo.descriptorSetCount = SwapChain::MAX_FRAMES_IN_FLIGHT;
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

    if (vkAllocateDescriptorSets(device.device(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descripot sets!");
    }

    for (size_t i = 0; i < SwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkDescriptorImageInfo imageInfo = model->imageInfo();

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

void RenderSystem::createUniformBuffers() 
{
    VkDeviceSize size = sizeof(UniformBufferObject);

    uniformBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMemory.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMapped.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < SwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
        device.createBuffer(
            size, 
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
            &uniformBuffers[i], 
            &uniformBuffersMemory[i]);

        vkMapMemory(device.device(), uniformBuffersMemory[i], 0, size, 0, &uniformBuffersMapped[i]);
    }
}

void RenderSystem::render(VkCommandBuffer commandBuffer, size_t currentFrame, float aspectRatio) const
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

    UniformBufferObject ubo;
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f),glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;

    memcpy(uniformBuffersMapped[currentFrame], &ubo, sizeof(ubo));

    VkDescriptorImageInfo imageInfo = model->imageInfo();
    updateDescriptorSets(currentFrame, imageInfo);

    vkCmdBindDescriptorSets(
        commandBuffer, 
        VK_PIPELINE_BIND_POINT_GRAPHICS, 
        pipelineLayout, 
        0, 1, 
        &descriptorSets[currentFrame], 
        0, nullptr);

    model->bind(commandBuffer);
    model->draw(commandBuffer);
}
        
void RenderSystem::updateDescriptorSets(size_t currentFrame, VkDescriptorImageInfo& imageInfo) const
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
