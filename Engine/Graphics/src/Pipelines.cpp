#include "pch.h"
#include "Pipelines.h"
#include "RenderDefines.h"
#include "RendUtil.h"
#include "Debug.h"


Pipelines::Pipelines(Descriptors* descriptors)
    : _descriptors(descriptors)
{
}

void Pipelines::Init(VkDevice device) {
    InitBackgroundPipelines(device);
}

void Pipelines::InitBackgroundPipelines(VkDevice device) {
    GradientComputePipeline(device);
}

void Pipelines::GradientComputePipeline(VkDevice device) {
    VkPipelineLayoutCreateInfo computeLayout = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .setLayoutCount = 1,
        .pSetLayouts = &_descriptors->_drawImageDescriptorLayout,
    };

    VK_CHECK(vkCreatePipelineLayout(device, &computeLayout, nullptr, &_gradientPipelineLayout));

    VkShaderModule computeDrawShader;
    if (!rutil::LoadShaderModule("Shaders/GLSL/gradient.comp.spv", device, &computeDrawShader))
    {
        Debug::Log(LogLevel::ERROR, "Failed to load compute shader");
    }

    VkPipelineShaderStageCreateInfo stageInfo = {};
    stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stageInfo.pNext = nullptr;
    stageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    stageInfo.module = computeDrawShader;
    stageInfo.pName = "main";

    VkComputePipelineCreateInfo computePipelineCreateInfo{};
    computePipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    computePipelineCreateInfo.pNext = nullptr;
    computePipelineCreateInfo.layout = _gradientPipelineLayout;
    computePipelineCreateInfo.stage = stageInfo;

    VK_CHECK(vkCreateComputePipelines(device,VK_NULL_HANDLE,1,&computePipelineCreateInfo, nullptr, &_gradientPipeline));
}
