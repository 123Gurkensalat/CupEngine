#pragma once

#include "device.hpp"
#include "swap_chain.hpp"
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace cup 
{     
    struct PipelineConfigInfo {
        PipelineConfigInfo(const PipelineConfigInfo&) = delete;
        PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

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
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };   
    
    class Pipeline {
    public:
        Pipeline(Device& device, SwapChain& swapChain, PipelineConfigInfo& configInfo);
        ~Pipeline();

        static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

        inline VkPipeline pipeline() { return pipeline_; }
        inline VkPipelineLayout pipelineLayout() { return pipelineLayout_; }

    private:
        void createPipelineLayout();
        void createPipeline(const PipelineConfigInfo& configInfo);
        VkShaderModule createShaderModule(const std::vector<char>& code);

        Device& device;
        SwapChain& swapChain;

        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;

        VkPipelineLayout pipelineLayout_;
        VkPipeline pipeline_;
    };
}
