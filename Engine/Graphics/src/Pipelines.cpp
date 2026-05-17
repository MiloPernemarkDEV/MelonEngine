#include "pch.h"
#include "Pipelines.h"
#include "RenderDefines.h"
#include "RendUtil.h"
#include "Debug.h"
#include "RenderTypes.h"
#include "PipelineBuilder.h"

Pipelines::Pipelines(Descriptors* descriptors, DeleteQueue* mainDeletetionQueue, Device* device)
    : _descriptors(descriptors), _mainDeletionQueue(mainDeletetionQueue), _device(device)
{
}

void Pipelines::Init() {
    InitBackgroundPipelines();
}

void Pipelines::InitBackgroundPipelines() {
    ComputePipeline();
    TrianglePipeline();

    _mainDeletionQueue->PushFunction([&]() {
        vkDestroyPipelineLayout(_device->GetDevice(), _gradientPipelineLayout, nullptr);
        vkDestroyPipeline(_device->GetDevice(), _gradientPipeline, nullptr);
        vkDestroyPipeline(_device->GetDevice(), _skyPipeline, nullptr);
        vkDestroyPipelineLayout(_device->GetDevice(), _trianglePipelineLayout, nullptr);
        vkDestroyPipeline(_device->GetDevice(), _trianglePipeline, nullptr);
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

    VK_CHECK(vkCreatePipelineLayout(_device->GetDevice(), &computeLayout, nullptr, &_gradientPipelineLayout));

    VkShaderModule gradientShader;
    if (!rutil::LoadShaderModule("Shaders/GLSL/gradient.comp.spv", _device->GetDevice(), &gradientShader))
    {
        Debug::Log(LogLevel::ERROR, "Failed to load compute shader");
    }

    VkShaderModule skyShader;
    if (!rutil::LoadShaderModule("Shaders/GLSL/sky.comp.spv", _device->GetDevice(), &skyShader))
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

    VK_CHECK(vkCreateComputePipelines(_device->GetDevice(),VK_NULL_HANDLE,1,&computePipelineCreateInfo, nullptr, &gradient.pipeline));

    computePipelineCreateInfo.stage.module = skyShader;
    ComputeEffect sky;
    sky.layout = _gradientPipelineLayout;
    sky.name = "sky";
    sky.data = {};

    sky.data.data1 = Vec4f(0.1, 0.2, 0.4 ,0.97);

    VK_CHECK(vkCreateComputePipelines(_device->GetDevice(), VK_NULL_HANDLE, 1, &computePipelineCreateInfo, nullptr, &sky.pipeline));

    _backgroundEffects.push_back(gradient);
    _backgroundEffects.push_back(sky);

    vkDestroyShaderModule(_device->GetDevice(), gradientShader, nullptr);
    vkDestroyShaderModule(_device->GetDevice(), skyShader, nullptr);

}

void Pipelines::TrianglePipeline() {
    VkShaderModule triangleVertexShader;
    if (!rutil::LoadShaderModule("Shaders/GLSL/colored_triangle.vert.spv", _device->GetDevice(), &triangleVertexShader)) {
        Debug::Log(LogLevel::ERROR, "Failed to load vertex shader");
    }
    VkShaderModule triangleFragmentShader;
    if (!rutil::LoadShaderModule("Shaders/GLSL/colored_triangle.frag.spv", _device->GetDevice(), &triangleFragmentShader)) {
       Debug::Log(LogLevel::ERROR, "Failed to load fragment shader");
   }

   VkPipelineLayoutCreateInfo pipeInfo = rutil::PipelineLayoutCreateInfo();
   VK_CHECK(vkCreatePipelineLayout(_device->GetDevice(), &pipeInfo, nullptr, &_trianglePipelineLayout));

    PipelineBuilder pipelineBuilder;
    pipelineBuilder._pipelineLayout = _trianglePipelineLayout;
    pipelineBuilder.SetShaders(triangleVertexShader, triangleFragmentShader);
    pipelineBuilder.SetInputTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
    pipelineBuilder.SetPolygonMode(VK_POLYGON_MODE_FILL);
    pipelineBuilder.SetCullMode(VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE);
    pipelineBuilder.SetMultisamplingModeNone();
    pipelineBuilder.DisableBlending();
    pipelineBuilder.DisableDepthTest();
    pipelineBuilder.SetColorAttachmentFormat(_device->_drawImage.imageFormat);
    pipelineBuilder.SetDepthFormat(VK_FORMAT_UNDEFINED);

    _trianglePipeline = pipelineBuilder.BuildPipeline(_device->GetDevice());

    vkDestroyShaderModule(_device->GetDevice(), triangleVertexShader, nullptr);
    vkDestroyShaderModule(_device->GetDevice(), triangleFragmentShader, nullptr);
}