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
        struct Vertex;

        Model(Device& device, Renderer& renderer);
        ~Model();

        Model(const Model&) = delete;
        Model &operator=(const Model&) = delete;

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

    private:
        void createVertexBuffer();

        Device& device;
        Renderer& renderer;

        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;

        const std::vector<Vertex> vertices;
    };

    struct Model::Vertex {
        glm::vec2 position;
        glm::vec3 color;

        static VkVertexInputBindingDescription getBindingDescription();
        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
    };
}
