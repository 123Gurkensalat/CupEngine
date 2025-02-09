#pragma once

#include "device.hpp"
#include "pipeline.hpp"
#include "swap_chain.hpp"
#include <memory>
#include <vulkan/vulkan_core.h>

namespace cup 
{
    class Renderer;
    class RenderSystem {
    public:
        RenderSystem(Device& device);

    protected:
        void createDescriptorSetLayout(std::vector<VkDescriptorSetLayoutBinding> bindings, VkDescriptorSetLayout* descriptorSetLayout);
        void createPipelineLayout(std::vector<VkDescriptorSetLayout> setLayout, std::vector<VkPushConstantRange> pushConstantRanges, VkPipelineLayout* pipelineLayout);
        void createPipeline(PipelineConfigInfo& pipelineConfig);

        template<typename T>
        void createUniformBuffers(std::vector<VkBuffer>& uniformBuffers, std::vector<VkDeviceMemory>& uniformBuffersMemory, std::vector<void*>& uniformBuffersMapped); 
        void createDescriptorSets(VkDescriptorSetLayout layout, std::vector<VkDescriptorSet>& descriptorSets);

        std::unique_ptr<Pipeline> pipeline;

    private:
        Device& device;
    };

    template<typename  T>
    void RenderSystem::createUniformBuffers(std::vector<VkBuffer>& uniformBuffers, std::vector<VkDeviceMemory>& uniformBuffersMemory, std::vector<void*>& uniformBuffersMapped) 
    {
        VkDeviceSize size = sizeof(T);

        uniformBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
        uniformBuffersMemory.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
        uniformBuffersMapped.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < SwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
            device.createBuffer(
                size, 
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                &uniformBuffers[i], 
                &uniformBuffersMemory[i]);

            vkMapMemory(device.device(), uniformBuffersMemory[i], 0, size, 0, &uniformBuffersMapped[i]);
        }
    }
}
