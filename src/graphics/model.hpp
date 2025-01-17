#pragma once

#include "graphics/device.hpp"
#include "graphics/swap_chain.hpp"
#include <array>
#include <glm/glm.hpp>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace cup 
{
    class Renderer;

    class Model {
    public:
        struct UniformBufferObject {
            glm::mat4 model;
            glm::mat4 view;
            glm::mat4 proj;
        };

        struct Vertex {
            glm::vec2 position;
            glm::vec3 color;

            static VkVertexInputBindingDescription getBindingDescription();
            static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
        };

        Model(Device& device, Renderer& renderer);
        ~Model();

        Model(const Model&) = delete;
        Model &operator=(const Model&) = delete;

        void updateUniformBuffer(uint32_t currentImage, const SwapChain& swapChain);

        void bind(VkCommandBuffer commandBuffer, uint32_t currentFrame);
        void draw(VkCommandBuffer commandBuffer);

    private:
        void createDescriptorPool();
        void createDescriptorSets();
        void createDeviceBuffer(
            VkDeviceSize size,
            const void* data,
            VkBuffer* dstBuffer, 
            VkDeviceMemory* dstBufferMemory,
            VkBufferUsageFlags usage);

        void createUniformBuffers(); 

        Device& device;
        Renderer& renderer;

        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;
       
        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;
        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        std::vector<void*> uniformBuffersMapped;

        const std::vector<Vertex> vertices = {
            {{-0.5f, -0.5f}, {0.469f, 0.000f, 0.000f}},
            {{-0.5f,  0.5f}, {0.754f, 0.070f, 0.121f}},
            {{ 0.0f,  0.0f}, {0.992f, 0.941f, 0.835f}},
            {{ 0.5f,  0.5f}, {0.000f, 0.188f, 0.286f}},
            {{ 0.5f, -0.5f}, {0.400f, 0.608f, 0.737f}}
        };

        const std::vector<uint16_t> indices = {
            0, 2, 1, 
            4, 2, 0,
            3, 2, 4,
            1, 2, 3
        };
    };
}
