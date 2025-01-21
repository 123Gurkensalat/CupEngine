#include "model.hpp"
#include "renderer.hpp"
#include <array>
#include <cstring>
#include <vulkan/vulkan_core.h>

using cup::Model;
using Vertex = cup::Model::Vertex;

Model::Model(Device& device, Renderer& renderer) : device(device), renderer(renderer)
{
    VkDeviceSize vertexBufferSize = sizeof(vertices[0]) * vertices.size();
    VkDeviceSize indexBufferSize = sizeof(indices[0]) * indices.size();

    createDeviceBuffer(vertexBufferSize, vertices.data(), &vertexBuffer, &vertexBufferMemory, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    createDeviceBuffer(indexBufferSize, indices.data(), &indexBuffer, &indexBufferMemory, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
}

Model::~Model()
{
    vkDestroyBuffer(device.device(), indexBuffer, nullptr);
    vkFreeMemory(device.device(), indexBufferMemory, nullptr);

    vkDestroyBuffer(device.device(), vertexBuffer, nullptr);
    vkFreeMemory(device.device(), vertexBufferMemory, nullptr);
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
        
void Model::bind(VkCommandBuffer commandBuffer) const
{
    VkBuffer vertexBuffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);
}

void Model::draw(VkCommandBuffer commandBuffer) const
{
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
}
