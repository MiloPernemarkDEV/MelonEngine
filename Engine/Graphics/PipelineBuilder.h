#ifndef MELONENGINE_PIPELINEBUILDER_H
#define MELONENGINE_PIPELINEBUILDER_H

#include <vulkan\vulkan.h>
#include <vector>

class PipelineBuilder {
public:
    PipelineBuilder() { clear(); };
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
    VkPipeline build_pipeline(VkDevice device);
    void clear();
    void set_shaders(VkShaderModule vertexShader, VkShaderModule fragmentShader);
    void set_set_input_topology(VkPrimitiveTopology topology);
    void set_polygon_mode(VkPolygonMode polygonMode);
    void set_cull_mode(VkCullModeFlags cullMode, VkFrontFace frontFace);
    void set_multisampling_mode_none();
    void disable_blending();
    void set_color_attachment_format(VkFormat format);
    void disable_depth_test();
    void set_depth_format(VkFormat format);
};


#endif //MELONENGINE_PIPELINEBUILDER_H
