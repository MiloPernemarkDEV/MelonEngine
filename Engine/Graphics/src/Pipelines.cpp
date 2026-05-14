#include "pch.h"
#include "Pipelines.h"
#include "RenderDefines.h"
#include "RendUtil.h"
#include "Debug.h"


Pipelines::Pipelines(Descriptors* descriptors, DeleteQueue* mainDeletetionQueue, VkDevice* device)
    : _descriptors(descriptors), _mainDeletionQueue(mainDeletetionQueue), _device(device)
{
}

void Pipelines::Init() {
    InitBackgroundPipelines();
}

void Pipelines::InitBackgroundPipelines() {
    GradientComputePipeline();

    _mainDeletionQueue->PushFunction([&]() {
        vkDestroyPipelineLayout(*_device, _gradientPipelineLayout, nullptr);
        vkDestroyPipeline(*_device, _gradientPipeline, nullptr);
    });
}

void Pipelines::GradientComputePipeline() {
    VkPipelineLayoutCreateInfo computeLayout = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .setLayoutCount = 1,
        .pSetLayouts = &_descriptors->_drawImageDescriptorLayout,
    };

    VK_CHECK(vkCreatePipelineLayout(*_device, &computeLayout, nullptr, &_gradientPipelineLayout));

    VkShaderModule computeDrawShader;
    if (!rutil::LoadShaderModule("Shaders/HLSL/gradient_compute.hlsl.spv", *_device, &computeDrawShader))
    {
        Debug::Log(LogLevel::ERROR, "Failed to load compute shader");
    }

    VkPipelineShaderStageCreateInfo stageInfo = {};
    stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stageInfo.pNext = nullptr;
    stageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    stageInfo.module = computeDrawShader;
    stageInfo.pName = "CSmain";

    VkComputePipelineCreateInfo computePipelineCreateInfo{};
    computePipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    computePipelineCreateInfo.pNext = nullptr;
    computePipelineCreateInfo.layout = _gradientPipelineLayout;
    computePipelineCreateInfo.stage = stageInfo;

    VK_CHECK(vkCreateComputePipelines(*_device,VK_NULL_HANDLE,1,&computePipelineCreateInfo, nullptr, &_gradientPipeline));
    vkDestroyShaderModule(*_device, computeDrawShader, nullptr);
}
