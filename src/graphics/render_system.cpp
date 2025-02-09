#include "render_system.hpp"
#include "pipeline.hpp"
#include "swap_chain.hpp"
#include <memory>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

using namespace cup;

RenderSystem::RenderSystem(Device& device) : device(device){}

void RenderSystem::createDescriptorSetLayout(std::vector<VkDescriptorSetLayoutBinding> bindings, VkDescriptorSetLayout* descriptorSetLayout) 
{
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(device.device(), &layoutInfo, nullptr, descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

void RenderSystem::createPipelineLayout(std::vector<VkDescriptorSetLayout> setLayout, std::vector<VkPushConstantRange> pushConstantRanges, VkPipelineLayout* pipelineLayout) 
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(setLayout.size());
    pipelineLayoutInfo.pSetLayouts = setLayout.data();
    pipelineLayoutInfo.pushConstantRangeCount = static_cast<uint32_t>(pushConstantRanges.size());
    pipelineLayoutInfo.pPushConstantRanges = pushConstantRanges.data();

    if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("could not create pipeline layout!");
    }
}

void RenderSystem::createPipeline(PipelineConfigInfo& pipelineConfig) 
{
    pipeline = std::make_unique<Pipeline>(device, pipelineConfig);
}

void RenderSystem::createDescriptorSets(VkDescriptorSetLayout layout, std::vector<VkDescriptorSet>& descriptorSets) 
{
    std::vector<VkDescriptorSetLayout> layouts{SwapChain::MAX_FRAMES_IN_FLIGHT, layout};
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = device.descriptorPool();
    allocInfo.descriptorSetCount = SwapChain::MAX_FRAMES_IN_FLIGHT;
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

    if (vkAllocateDescriptorSets(device.device(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descripot sets!");
    }
}
