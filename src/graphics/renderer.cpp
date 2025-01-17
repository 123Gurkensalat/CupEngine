#include "renderer.hpp"
#include "graphics/device.hpp"
#include "graphics/window.hpp"
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

using cup::Renderer;

Renderer::Renderer(cup::Device& device, cup::Window& window) : device(device), window(window), swapChain(device, window)
{
    QueueFamilyIndices queueFamilyIndices = device.getPhysicalQueueFamilies();
    createPipeline();
    createCommandPool(queueFamilyIndices.graphicsFamily.value(), &graphicsCommandPool);
    createCommandPool(queueFamilyIndices.transferFamily.value(), &transferCommandPool);
    createCommandBuffer();
    model = std::make_unique<Model>(device, *this);
}

Renderer::~Renderer()
{
    vkDestroyCommandPool(device.device(), graphicsCommandPool, nullptr);
    vkDestroyCommandPool(device.device(), transferCommandPool, nullptr);
}

void Renderer::drawFrame()
{
    uint32_t imageIndex;
    auto result = swapChain.acquireNextImage(currentFrame, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        swapChain.recreateSwapChain();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    model->updateUniformBuffer(currentFrame, swapChain);

    vkResetCommandBuffer(commandBuffers[currentFrame], 0);

    recordCommandBuffer(commandBuffers[currentFrame], imageIndex);

    result = swapChain.submitCommandBuffer(currentFrame, commandBuffers[currentFrame], imageIndex);    

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasResized()) {
        window.resetResizedFlag();
        swapChain.recreateSwapChain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
}

bool Renderer::finished() 
{
    return swapChain.fencesFinished();
}

void Renderer::createPipeline() 
{
    PipelineConfigInfo pipelineConfig{};
    Pipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.vertShaderFilePath = "../src/graphics/shader/bin/default.vert.spv"; 
    pipelineConfig.fragShaderFilePath = "../src/graphics/shader/bin/default.frag.spv"; 
    pipeline_ = std::make_unique<Pipeline>(device, swapChain, pipelineConfig);
}

void Renderer::createCommandPool(uint32_t queueFamilyIndex, VkCommandPool* commandPool)
{
    VkCommandPoolCreateInfo commandPoolInfo{};
    commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    commandPoolInfo.queueFamilyIndex = queueFamilyIndex;
    
    if (vkCreateCommandPool(device.device(), &commandPoolInfo, nullptr, commandPool) != VK_SUCCESS) {
        throw std::runtime_error("could not create command pool!");
    }
}

void Renderer::createCommandBuffer() 
{
    commandBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = graphicsCommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("could not allocate command buffers!");
    }
}

VkCommandBuffer Renderer::beginTransferCommands()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = transferCommandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device.device(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void Renderer::endTransferCommands(VkCommandBuffer commandBuffer) 
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(device.transferQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(device.transferQueue());

    vkFreeCommandBuffers(device.device(), transferCommandPool, 1, &commandBuffer);
}

void Renderer::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) 
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("could not begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = swapChain.renderPass();
    renderPassInfo.framebuffer = swapChain.framebuffers()[imageIndex];

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChain.extent();

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_->pipeline());

    model->bind(commandBuffer, currentFrame);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swapChain.extent().width);
    viewport.height = static_cast<float>(swapChain.extent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.extent = swapChain.extent();
    scissor.offset = {0, 0};
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    model->draw(commandBuffer);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record commandBuffer!");
    }
}
