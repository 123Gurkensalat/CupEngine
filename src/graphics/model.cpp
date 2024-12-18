#include  "model.hpp"
#include <array>
#include <cstring>
#include <vulkan/vulkan_core.h>

using cup::Model;
using Vertex = cup::Model::Vertex;

Model::Model(Device& device) : device(device), 
    vertices({
        {{ 0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{ 0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}},
    }) 
{
    createVertexBuffer();
}

Model::~Model()
{
    vkDestroyBuffer(device.device(), vertexBuffer, nullptr);
    vkFreeMemory(device.device(), vertexBufferMemory, nullptr);
}

void Model::createVertexBuffer() 
{
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    device.createBuffer(
        bufferSize, 
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
        &vertexBuffer, 
        &vertexBufferMemory);

    void* data;
    vkMapMemory(device.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
    vkUnmapMemory(device.device(), vertexBufferMemory);
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
        
void Model::bind(VkCommandBuffer commandBuffer) 
{
    VkBuffer vertexBuffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
}

void Model::draw(VkCommandBuffer commandBuffer)
{
    vkCmdDraw(commandBuffer, static_cast<uint32_t>(vertices.size()), 1, 0, 0);
}
