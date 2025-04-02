#pragma once

#include "graphics/device.hpp"
#include <array>
#include <glm/ext/vector_float2.hpp>
#include <vulkan/vulkan_core.h>

namespace cup {
    class Sprite {
    public:
        struct Vertex {
            glm::vec2 position;
            glm::vec2 texCoord;

            static VkVertexInputBindingDescription getBindingDescription();
            static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
        };

        Sprite() = delete;
        Sprite(Device* device, const char* path);
        Sprite(const Sprite&) = delete;
        Sprite(Sprite&&) = default;

        ~Sprite();

        Sprite& operator=(const Sprite&) = delete;
        Sprite& operator=(Sprite&&) = delete;

        VkDescriptorImageInfo imageInfo() const;

        void bind(VkCommandBuffer commandBuffer) const;
        void draw(VkCommandBuffer commandBuffer) const;

    private: 
        VkExtent2D createTextureImage(const char* path);
        void createTextureImageView();
        void createTextureSampler();

        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) const;
        void copyBufferToImage(VkBuffer buffer, VkImage image, VkExtent3D extent) const;

        Device* device;

        // 2 ---- 3
        // | \    |
        // |    \ |
        // 0 ---- 1
        std::array<Vertex, 4> vertices; 
        const uint32_t pixelsToUnits = 512;

        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;

        VkImage textureImage;
        VkDeviceMemory textureImageMemory; 
        VkImageView textureImageView;
        VkSampler textureSampler;
    };
}
