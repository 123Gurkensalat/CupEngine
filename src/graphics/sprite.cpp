#include "sprite.hpp"

#include <stb_image.h>
#include <stdexcept>
#include <cstring>
#include <string>
#include <vulkan/vulkan_core.h>

using namespace cup;

typedef uint16_t vertexIndex;
constexpr std::array<vertexIndex, 6> vertexIndices = {0, 1, 2, 1, 3, 2};

Sprite::Sprite(Device& device, const std::string& path) : device(device)
{
    VkDeviceSize vertexBufferSize = sizeof(vertices[0]) * vertices.size();
    constexpr VkDeviceSize indexBufferSize = sizeof(vertexIndices[0]) * vertexIndices.size();

    VkExtent2D texExtent = createTextureImage(path);
    createTextureImageView();
    createTextureSampler();

    float unitsWidth = (float)texExtent.width / pixelsToUnits / 2;
    float unitsHeight = (float)texExtent.height / pixelsToUnits / 2;
    vertices = {
        Vertex{{-unitsWidth, -unitsHeight}, {0.0f, 0.0f}},
        Vertex{{ unitsWidth, -unitsHeight}, {1.0f, 0.0f}},
        Vertex{{-unitsWidth,  unitsHeight}, {0.0f, 1.0f}},
        Vertex{{ unitsWidth,  unitsHeight}, {1.0f, 1.0f}}
    };

    device.createTransferBuffer(vertexBufferSize, vertices.data(), &vertexBuffer, &vertexBufferMemory, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    device.createTransferBuffer(indexBufferSize, vertexIndices.data(), &indexBuffer, &indexBufferMemory, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
}

Sprite::~Sprite() 
{    
    vkDestroySampler(device.device(), textureSampler, nullptr);
    vkDestroyImageView(device.device(), textureImageView, nullptr);
    vkDestroyImage(device.device(), textureImage, nullptr);
    vkFreeMemory(device.device(), textureImageMemory, nullptr);
 
    vkDestroyBuffer(device.device(), indexBuffer, nullptr);
    vkFreeMemory(device.device(), indexBufferMemory, nullptr);

    vkDestroyBuffer(device.device(), vertexBuffer, nullptr);
    vkFreeMemory(device.device(), vertexBufferMemory, nullptr);
}
        
VkDescriptorImageInfo Sprite::imageInfo() const 
{
    VkDescriptorImageInfo imageInfo;
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = textureImageView;
    imageInfo.sampler = textureSampler;
    return imageInfo;
}

void Sprite::bind(VkCommandBuffer commandBuffer) const
{
    VkBuffer vertexBuffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);
}

void Sprite::draw(VkCommandBuffer commandBuffer) const
{
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(vertexIndices.size()), 1, 0, 0, 0);
}  

VkExtent2D Sprite::createTextureImage(const std::string& path)
{
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels) {
        throw std::runtime_error("failed to load texture image!");
    }

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    device.createBuffer(
        imageSize, 
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &stagingBuffer,
        &stagingBufferMemory);

    void* data;
    vkMapMemory(device.device(), stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(device.device(), stagingBufferMemory);

    stbi_image_free(pixels);

    device.createImage(
        {static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight), 1}, 
        VK_FORMAT_R8G8B8A8_SRGB, 
        VK_IMAGE_TILING_OPTIMAL, 
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
        &textureImage, 
        &textureImageMemory);

    transitionImageLayout(
        textureImage, 
        VK_FORMAT_R8G8B8A8_SRGB, 
        VK_IMAGE_LAYOUT_UNDEFINED, 
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    copyBufferToImage(
        stagingBuffer, 
        textureImage, 
        {static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight), 1});

    transitionImageLayout(
        textureImage, 
        VK_FORMAT_R8G8B8A8_SRGB, 
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(device.device(), stagingBuffer, nullptr);
    vkFreeMemory(device.device(), stagingBufferMemory, nullptr);

    return {static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight)};
}

void Sprite::createTextureImageView() 
{
    device.createImageView(textureImage, VK_FORMAT_R8G8B8A8_SRGB, &textureImageView);
}

void Sprite::createTextureSampler() 
{
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    if (vkCreateSampler(device.device(), &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture sampler!");
    }
}

void Sprite::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) const
{
    VkCommandBuffer commandBuffer = device.beginTransferCommands();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else {
        throw std::runtime_error("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(
        commandBuffer, 
        sourceStage, destinationStage, 
        0, 
        0, nullptr,
        0, nullptr, 
        1, &barrier);

    device.endTransferCommands(commandBuffer);
}

void Sprite::copyBufferToImage(VkBuffer buffer, VkImage image, VkExtent3D extent) const
{
    VkCommandBuffer commandBuffer = device.beginTransferCommands();

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = extent;

    vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    device.endTransferCommands(commandBuffer);
}

VkVertexInputBindingDescription Sprite::Vertex::getBindingDescription() 
{
    VkVertexInputBindingDescription bindingDesciption{};
    bindingDesciption.binding = 0;
    bindingDesciption.stride = sizeof(Sprite::Vertex);
    bindingDesciption.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDesciption;
}

std::array<VkVertexInputAttributeDescription, 2> Sprite::Vertex::getAttributeDescriptions() 
{
    std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, texCoord);

    return attributeDescriptions;
}
