#include "pch.h"
#include "Pipelines.h"
#include "RenderDefines.h"
#include "RendUtil.h"
#include "../core/Debug.h"
#include "RenderTypes.h"
#include "PipelineBuilder.h"

Pipelines::Pipelines(Descriptors* descriptors, DeleteQueue* mainDeletetionQueue, Device* device)
    : _descriptors(descriptors), _mainDeletionQueue(mainDeletetionQueue), _device(device)
{
}

void Pipelines::Init() {
    InitBackgroundPipelines();
}

void Pipelines::MeshPipeline() {
    VkShaderModule triangleFragShader;
    if (!rutil::LoadShaderModule("Shaders/GLSL/colored_triangle.frag.spv", _device->GetDevice(), &triangleFragShader)) {
        Debug::log(LogLevel::ERROR,"Error when building the triangle fragment shader module");
    }

    VkShaderModule triangleVertexShader;
    if (!rutil::LoadShaderModule("Shaders/GLSL/colored_triangle_mesh.vert.spv", _device->GetDevice(), &triangleVertexShader)) {
        Debug::log(LogLevel::ERROR,"Error when building the triangle vertex shader module");
    }

    VkPushConstantRange bufferRange{};
    bufferRange.offset = 0;
    bufferRange.size = sizeof(GPUDrawPushConstants);
    bufferRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkPipelineLayoutCreateInfo pipeline_layout_info = rutil::PipelineLayoutCreateInfo();
    pipeline_layout_info.pPushConstantRanges = &bufferRange;
    pipeline_layout_info.pushConstantRangeCount = 1;

    VK_CHECK(vkCreatePipelineLayout(_device->GetDevice(), &pipeline_layout_info, nullptr, &_meshPipelineLayout));

    PipelineBuilder pipelineBuilder;
    pipelineBuilder._pipelineLayout = _meshPipelineLayout;
    pipelineBuilder.set_shaders(triangleVertexShader, triangleFragShader);
    pipelineBuilder.set_set_input_topology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
    pipelineBuilder.set_polygon_mode(VK_POLYGON_MODE_FILL);
    pipelineBuilder.set_cull_mode(VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE);
    pipelineBuilder.set_multisampling_mode_none();
    pipelineBuilder.disable_blending();

    pipelineBuilder.disable_depth_test();

    //connect the image format we will draw into, from draw image
    pipelineBuilder.set_color_attachment_format(_device->_drawImage.imageFormat);
    pipelineBuilder.set_depth_format(VK_FORMAT_UNDEFINED);

    //finally build the pipeline
    _meshPipeline = pipelineBuilder.build_pipeline(_device->GetDevice());

    //clean structures
    vkDestroyShaderModule(_device->GetDevice(), triangleFragShader, nullptr);
    vkDestroyShaderModule(_device->GetDevice(), triangleVertexShader, nullptr);
}

void Pipelines::InitBackgroundPipelines() {
    ComputePipeline();
    TrianglePipeline();
    MeshPipeline();

    _mainDeletionQueue->PushFunction([&]() {
        vkDestroyPipelineLayout(_device->GetDevice(), _gradientPipelineLayout, nullptr);
        vkDestroyPipeline(_device->GetDevice(), _gradientPipeline, nullptr);
        vkDestroyPipeline(_device->GetDevice(), _skyPipeline, nullptr);
        vkDestroyPipelineLayout(_device->GetDevice(), _trianglePipelineLayout, nullptr);
        vkDestroyPipeline(_device->GetDevice(), _trianglePipeline, nullptr);
        vkDestroyPipelineLayout(_device->GetDevice(), _meshPipelineLayout, nullptr);
        vkDestroyPipeline(_device->GetDevice(), _meshPipeline, nullptr);
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
        Debug::log(LogLevel::ERROR, "Failed to load compute shader");
    }

    VkShaderModule skyShader;
    if (!rutil::LoadShaderModule("Shaders/GLSL/sky.comp.spv", _device->GetDevice(), &skyShader))
    {
        Debug::log(LogLevel::ERROR, "Failed to load compute shader");
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
    gradient.data.data1 = vec4f(1, 0, 0, 1);
    gradient.data.data2 = vec4f(0, 0, 1, 1);

    VK_CHECK(vkCreateComputePipelines(_device->GetDevice(),VK_NULL_HANDLE,1,&computePipelineCreateInfo, nullptr, &gradient.pipeline));

    computePipelineCreateInfo.stage.module = skyShader;
    ComputeEffect sky;
    sky.layout = _gradientPipelineLayout;
    sky.name = "sky";
    sky.data = {};

    sky.data.data1 = vec4f(0.1, 0.2, 0.4 ,0.97);

    VK_CHECK(vkCreateComputePipelines(_device->GetDevice(), VK_NULL_HANDLE, 1, &computePipelineCreateInfo, nullptr, &sky.pipeline));

    _backgroundEffects.push_back(gradient);
    _backgroundEffects.push_back(sky);

    vkDestroyShaderModule(_device->GetDevice(), gradientShader, nullptr);
    vkDestroyShaderModule(_device->GetDevice(), skyShader, nullptr);

}

void Pipelines::TrianglePipeline() {
    VkShaderModule triangleVertexShader;
    if (!rutil::LoadShaderModule("Shaders/GLSL/colored_triangle.vert.spv", _device->GetDevice(), &triangleVertexShader)) {
        Debug::log(LogLevel::ERROR, "Failed to load vertex shader");
    }
    VkShaderModule triangleFragmentShader;
    if (!rutil::LoadShaderModule("Shaders/GLSL/colored_triangle.frag.spv", _device->GetDevice(), &triangleFragmentShader)) {
       Debug::log(LogLevel::ERROR, "Failed to load fragment shader");
   }

   VkPipelineLayoutCreateInfo pipeInfo = rutil::PipelineLayoutCreateInfo();
   VK_CHECK(vkCreatePipelineLayout(_device->GetDevice(), &pipeInfo, nullptr, &_trianglePipelineLayout));

    PipelineBuilder pipelineBuilder;
    pipelineBuilder._pipelineLayout = _trianglePipelineLayout;
    pipelineBuilder.set_shaders(triangleVertexShader, triangleFragmentShader);
    pipelineBuilder.set_set_input_topology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
    pipelineBuilder.set_polygon_mode(VK_POLYGON_MODE_FILL);
    pipelineBuilder.set_cull_mode(VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE);
    pipelineBuilder.set_multisampling_mode_none();
    pipelineBuilder.disable_blending();
    pipelineBuilder.disable_depth_test();
    pipelineBuilder.set_color_attachment_format(_device->_drawImage.imageFormat);
    pipelineBuilder.set_depth_format(VK_FORMAT_UNDEFINED);

    _trianglePipeline = pipelineBuilder.build_pipeline(_device->GetDevice());

    vkDestroyShaderModule(_device->GetDevice(), triangleVertexShader, nullptr);
    vkDestroyShaderModule(_device->GetDevice(), triangleFragmentShader, nullptr);
}