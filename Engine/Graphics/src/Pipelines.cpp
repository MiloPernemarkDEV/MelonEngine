#include "pch.h"
#include "Pipelines.h"
#include "RenderDefines.h"
#include "RendUtil.h"
#include "Debug.h"
#include "VulkanTypes.h"


Pipelines::Pipelines(Descriptors* descriptors, DeleteQueue* mainDeletetionQueue, VkDevice* device)
    : _descriptors(descriptors), _mainDeletionQueue(mainDeletetionQueue), _device(device)
{
}

void Pipelines::Init() {
    InitBackgroundPipelines();
}

void Pipelines::InitBackgroundPipelines() {
    ComputePipeline();

    _mainDeletionQueue->PushFunction([&]() {
        vkDestroyPipelineLayout(*_device, _gradientPipelineLayout, nullptr);
        vkDestroyPipeline(*_device, _gradientPipeline, nullptr);
        vkDestroyPipeline(*_device, _skyPipeline, nullptr);
    });
}

void Pipelines::ComputePipeline() {
    VkPushConstantRange pushConstant{};
    pushConstant.offset = 0;
    pushConstant.size = sizeof(ComputePushConstants);
    pushConstant.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    VkPipelineLayoutCreateInfo computeLayout = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .setLayoutCount = 1,
        .pSetLayouts = &_descriptors->_drawImageDescriptorLayout,
        .pushConstantRangeCount = 1,
        .pPushConstantRanges = &pushConstant
    };

    VK_CHECK(vkCreatePipelineLayout(*_device, &computeLayout, nullptr, &_gradientPipelineLayout));

    VkShaderModule gradientShader;
    if (!rutil::LoadShaderModule("Shaders/GLSL/gradient.comp.spv", *_device, &gradientShader))
    {
        Debug::Log(LogLevel::ERROR, "Failed to load compute shader");
    }

    VkShaderModule skyShader;
    if (!rutil::LoadShaderModule("Shaders/GLSL/sky.comp.spv", *_device, &skyShader))
    {
        Debug::Log(LogLevel::ERROR, "Failed to load compute shader");
    }

    VkPipelineShaderStageCreateInfo stageInfo = {};
    stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stageInfo.pNext = nullptr;
    stageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    stageInfo.module = gradientShader;
    stageInfo.pName = "main";

    VkComputePipelineCreateInfo computePipelineCreateInfo{};
    computePipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    computePipelineCreateInfo.pNext = nullptr;
    computePipelineCreateInfo.layout = _gradientPipelineLayout;
    computePipelineCreateInfo.stage = stageInfo;

    ComputeEffect gradient;
    gradient.layout = _gradientPipelineLayout;
    gradient.name = "gradient";
    gradient.data = {};

    //default colors
    gradient.data.data1 = Vec4f(1, 0, 0, 1);
    gradient.data.data2 = Vec4f(0, 0, 1, 1);

    VK_CHECK(vkCreateComputePipelines(*_device,VK_NULL_HANDLE,1,&computePipelineCreateInfo, nullptr, &gradient.pipeline));

    computePipelineCreateInfo.stage.module = skyShader;
    ComputeEffect sky;
    sky.layout = _gradientPipelineLayout;
    sky.name = "sky";
    sky.data = {};

    sky.data.data1 = Vec4f(0.1, 0.2, 0.4 ,0.97);

    VK_CHECK(vkCreateComputePipelines(*_device, VK_NULL_HANDLE, 1, &computePipelineCreateInfo, nullptr, &sky.pipeline));

    _backgroundEffects.push_back(gradient);
    _backgroundEffects.push_back(sky);

    vkDestroyShaderModule(*_device, gradientShader, nullptr);
    vkDestroyShaderModule(*_device, skyShader, nullptr);

}


