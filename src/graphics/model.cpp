#include "model.hpp"
#include "graphics/swap_chain.hpp"
#include "renderer.hpp"
#include <glm/detail/qualifier.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>
#include <chrono>
#include <cstring>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

using cup::Model;
using Vertex = cup::Model::Vertex;

Model::Model(Device& device, Renderer& renderer) : device(device), renderer(renderer)
{
    VkDeviceSize vertexBufferSize = sizeof(vertices[0]) * vertices.size();
    VkDeviceSize indexBufferSize = sizeof(indices[0]) * indices.size();

    createDeviceBuffer(vertexBufferSize, vertices.data(), &vertexBuffer, &vertexBufferMemory, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    createDeviceBuffer(indexBufferSize, indices.data(), &indexBuffer, &indexBufferMemory, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    createUniformBuffers();

    createDescriptorPool();
    createDescriptorSets();
}

Model::~Model()
{
    vkDestroyDescriptorPool(device.device(), descriptorPool, nullptr);

    for (size_t i = 0; i < SwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroyBuffer(device.device(), uniformBuffers[i], nullptr);
        vkFreeMemory(device.device(), uniformBuffersMemory[i], nullptr);
    }

    vkDestroyBuffer(device.device(), indexBuffer, nullptr);
    vkFreeMemory(device.device(), indexBufferMemory, nullptr);

    vkDestroyBuffer(device.device(), vertexBuffer, nullptr);
    vkFreeMemory(device.device(), vertexBufferMemory, nullptr);
}

void Model::createDescriptorPool() 
{
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = static_cast<uint32_t>(SwapChain::MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = static_cast<uint32_t>(SwapChain::MAX_FRAMES_IN_FLIGHT);

    if (vkCreateDescriptorPool(device.device(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

void Model::createDescriptorSets() 
{
    std::vector<VkDescriptorSetLayout> layouts(SwapChain::MAX_FRAMES_IN_FLIGHT, renderer.pipeline().descriptorSetLayout());
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
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

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptorSets[i];
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;
        descriptorWrite.pImageInfo = nullptr;
        descriptorWrite.pTexelBufferView = nullptr;

        vkUpdateDescriptorSets(device.device(), 1, &descriptorWrite, 0, nullptr);
    }
}

void Model::createDeviceBuffer(
        VkDeviceSize size, 
        const void* data, 
        VkBuffer* dstBuffer, 
        VkDeviceMemory* dstBufferMemory, 
        VkBufferUsageFlags usage) 
{
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    device.createBuffer(
        size, 
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
        &stagingBuffer,
        &stagingBufferMemory);

    void* datas;
    vkMapMemory(device.device(), stagingBufferMemory, 0, size, 0, &datas);
    memcpy(datas, data, static_cast<size_t>(size));
    vkUnmapMemory(device.device(), stagingBufferMemory);

    device.createBuffer(
        size, 
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage, 
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
        dstBuffer, 
        dstBufferMemory);

    VkCommandBuffer commandBuffer = renderer.beginTransferCommands();
    device.copyBuffer(size, stagingBuffer, *dstBuffer, commandBuffer);
    renderer.endTransferCommands(commandBuffer);

    vkDestroyBuffer(device.device(), stagingBuffer, nullptr);
    vkFreeMemory(device.device(), stagingBufferMemory, nullptr);
}

void Model::createUniformBuffers() 
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

void Model::updateUniformBuffer(uint32_t currentImage, const SwapChain& swapChain) 
{
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo;
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f),glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), swapChain.extent().width / (float) swapChain.extent().height, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;

    memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

VkVertexInputBindingDescription Vertex::getBindingDescription() 
{
    VkVertexInputBindingDescription bindingDesciption{};
    bindingDesciption.binding = 0;
    bindingDesciption.stride = sizeof(Model::Vertex);
    bindingDesciption.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDesciption;
}

std::array<VkVertexInputAttributeDescription, 2> Vertex::getAttributeDescriptions() 
{
    std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    return attributeDescriptions;
}
        
void Model::bind(VkCommandBuffer commandBuffer, uint32_t currentFrame) 
{
    VkBuffer vertexBuffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);
    vkCmdBindDescriptorSets(
        commandBuffer, 
        VK_PIPELINE_BIND_POINT_GRAPHICS, 
        renderer.pipeline().pipelineLayout(), 
        0, 1, 
        &descriptorSets[currentFrame], 
        0, nullptr);
}

void Model::draw(VkCommandBuffer commandBuffer)
{
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
}
