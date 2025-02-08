#include "renderer.hpp"
#include "device.hpp"
#include "render_system.hpp"
#include "window.hpp"
#include <cstdint>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

using namespace cup;

Renderer::Renderer(Device& device, Window& window) 
    : device(device), window(window), swapChain_(device, window)
{
    createCommandBuffer();
    renderSystem = std::make_unique<RenderSystem>(device, *this);
}

void Renderer::createCommandBuffer() 
{
    commandBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = device.graphicsCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("could not allocate command buffers!");
    }
}

void Renderer::draw() 
{
    if (auto commandBuffer = beginFrame()) {
        beginSwapChainRenderPass(commandBuffer);

        renderSystem->render(commandBuffer, currentFrame, (float) swapChain_.extent().width / swapChain_.extent().height);

        endSwapChainRenderPass(commandBuffer);
        endFrame();
    }
}

VkCommandBuffer Renderer::beginFrame() 
{
    auto result = swapChain_.acquireNextImage(&currentImageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        swapChain_.recreateSwapChain();
        return nullptr;
    } 

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    auto commandBuffer = commandBuffers[currentFrame];

    vkResetCommandBuffer(commandBuffer, 0);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("could not begin recording command buffer!");
    }

    return commandBuffer;
}
        
void Renderer::endFrame() 
{
    auto commandBuffer = commandBuffers[currentFrame];

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record commandBuffer!");
    }

    auto result = swapChain_.submitCommandBuffer(commandBuffer, currentImageIndex);    

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasResized()) {
        window.resetResizedFlag();
        swapChain_.recreateSwapChain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
}

void Renderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) 
{
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = swapChain_.renderPass();
    renderPassInfo.framebuffer = swapChain_.framebuffers()[currentImageIndex];

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChain_.extent();

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swapChain_.extent().width);
    viewport.height = static_cast<float>(swapChain_.extent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.extent = swapChain_.extent();
    scissor.offset = {0, 0};
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}
    
void Renderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
    vkCmdEndRenderPass(commandBuffer);
}

