#ifndef MELONENGINE_PIPELINE_H
#define MELONENGINE_PIPELINE_H

#include <vulkan/vulkan.h>
#include "Descriptors.h"

class Pipelines {
public:
    Pipelines(Descriptors* descriptors);
    Pipelines() = default;

    VkPipeline _gradientPipeline;
    VkPipelineLayout _gradientPipelineLayout;

    void Init(VkDevice device);
private:
    void InitBackgroundPipelines(VkDevice device);
    void GradientComputePipeline(VkDevice device);
    Descriptors* _descriptors;
};

#endif //MELONENGINE_PIPELINE_H
