#pragma once

#include "device.hpp"

#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace cup {
struct PipelineConfigInfo {
    PipelineConfigInfo() = default;
    PipelineConfigInfo(const PipelineConfigInfo&) = delete;
    PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;
    PipelineConfigInfo(PipelineConfigInfo&&) = default;
    PipelineConfigInfo& operator=(PipelineConfigInfo&&) = default;

    std::string vertShaderFilePath;
    std::string fragShaderFilePath;

    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    std::vector<VkDynamicState> dynamicStateEnables;
    VkPipelineDynamicStateCreateInfo dynamicStateInfo;
    VkDescriptorSetLayout descriptorSetLayout = nullptr;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
};

class Pipeline {
public:
    Pipeline(Device& device, PipelineConfigInfo& configInfo);
    ~Pipeline();

    static PipelineConfigInfo defaultPipelineConfigInfo();

    VkPipeline pipeline() const { return pipeline_; }

    void bind(VkCommandBuffer) const;

private:
    void createPipeline(const PipelineConfigInfo& configInfo);
    VkShaderModule createShaderModule(const std::vector<char>& code);

    Device& device;

    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;

    VkPipeline pipeline_;
};
} // namespace cup
