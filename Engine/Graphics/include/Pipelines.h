#ifndef MELONENGINE_PIPELINE_H
#define MELONENGINE_PIPELINE_H

#include <vulkan/vulkan.h>
#include "Descriptors.h"
#include "VulkanTypes.h"

class Pipelines {
public:
    Pipelines(Descriptors* descriptors, DeleteQueue* mainDeletetionQueue, VkDevice* device);
    Pipelines() = default;

    VkPipeline _gradientPipeline;
    VkPipelineLayout _gradientPipelineLayout;

    void Init();
private:
    void InitBackgroundPipelines();
    void GradientComputePipeline();
    Descriptors* _descriptors;
    DeleteQueue* _mainDeletionQueue;
    VkDevice* _device;
};

#endif //MELONENGINE_PIPELINE_H
