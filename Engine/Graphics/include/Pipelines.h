#ifndef MELONENGINE_PIPELINE_H
#define MELONENGINE_PIPELINE_H

#include <vulkan/vulkan.h>
#include "Descriptors.h"
#include "VulkanTypes.h"

class Pipelines {
public:
    Pipelines(Descriptors* descriptors, DeleteQueue* mainDeletetionQueue, VkDevice* device);
    Pipelines() = default;

    std::vector<ComputeEffect>& GetBackgroundEffects() { return _backgroundEffects; }
    int& GetCurrentBackgroundEffect() { return currentBackgroundEffect; }

    VkPipeline _gradientPipeline = VK_NULL_HANDLE;
    VkPipeline _skyPipeline = VK_NULL_HANDLE;
    VkPipelineLayout _gradientPipelineLayout = VK_NULL_HANDLE;

    void Init();
private:
    void InitBackgroundPipelines();
    void ComputePipeline();
    Descriptors* _descriptors;
    DeleteQueue* _mainDeletionQueue;
    VkDevice* _device;
    std::vector<ComputeEffect> _backgroundEffects;
    int currentBackgroundEffect = 0;
};

#endif //MELONENGINE_PIPELINE_H
