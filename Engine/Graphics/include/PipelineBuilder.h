#ifndef MELONENGINE_PIPELINEBUILDER_H
#define MELONENGINE_PIPELINEBUILDER_H

#include <vulkan\vulkan.h>
#include <vector>

class PipelineBuilder {
public:
    PipelineBuilder() { Clear(); };
    std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;

    VkPipelineInputAssemblyStateCreateInfo _inputAssembly;
    VkPipelineRasterizationStateCreateInfo _rasterizer;
    VkPipelineColorBlendAttachmentState _colorBlendAttachment;
    VkPipelineMultisampleStateCreateInfo _multisampling;
    VkPipelineLayout _pipelineLayout;
    VkPipelineDepthStencilStateCreateInfo _depthStencil;
    VkPipelineRenderingCreateInfo _renderInfo;
    VkFormat _colorAttachmentformat;

    // API
    VkPipeline BuildPipeline(VkDevice device);
    void Clear();
    void SetShaders(VkShaderModule vertexShader, VkShaderModule fragmentShader);
    void SetInputTopology(VkPrimitiveTopology topology);
    void SetPolygonMode(VkPolygonMode polygonMode);
    void SetCullMode(VkCullModeFlags cullMode, VkFrontFace frontFace);
    void SetMultisamplingModeNone();
    void DisableBlending();
    void SetColorAttachmentFormat(VkFormat format);
    void DisableDepthTest();
};


#endif //MELONENGINE_PIPELINEBUILDER_H
