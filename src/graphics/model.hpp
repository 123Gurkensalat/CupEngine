#pragma once

#include "graphics/device.hpp"
#include <array>
#include <glm/glm.hpp>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace cup 
{
    class Renderer;

    class Model {
    public:
        struct Vertex {
            glm::vec2 position;
            glm::vec2 texCoord;

            static VkVertexInputBindingDescription getBindingDescription();
            static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
        };

        Model(Device& device, Renderer& renderer);
        ~Model();

        Model(const Model&) = delete;
        Model& operator=(const Model&) = delete;
        Model(Model&&) = delete;
        Model& operator=(Model&&) = delete;

        void bind(VkCommandBuffer commandBuffer) const;
        void draw(VkCommandBuffer commandBuffer) const;

    private:
        void createDeviceBuffer(
            VkDeviceSize size,
            const void* data,
            VkBuffer* dstBuffer, 
            VkDeviceMemory* dstBufferMemory,
            VkBufferUsageFlags usage);

        Device& device;
        Renderer& renderer;

        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;
       
        const std::vector<Vertex> vertices = {
            {{-0.5f, -0.5f}, {1.0f, 0.0f}},
            {{-0.5f,  0.5f}, {1.0f, 1.0f}},
            {{ 0.0f,  0.0f}, {0.5f, 0.5f}},
            {{ 0.5f,  0.5f}, {0.0f, 1.0f}},
            {{ 0.5f, -0.5f}, {0.0f, 0.0f}}
        };

        const std::vector<uint16_t> indices = {
            0, 2, 1, 
            4, 2, 0,
            3, 2, 4,
            1, 2, 3
        };
    };
}
