#ifndef MELONENGINE_PIPELINE_H
#define MELONENGINE_PIPELINE_H

#include <vulkan/vulkan.h>
#include "Descriptors.h"
#include "RenderTypes.h"
#include "Device.h"

class Pipelines {
public:
    Pipelines(Descriptors* descriptors, DeleteQueue* mainDeletetionQueue, Device* device);
    Pipelines() = default;

    std::vector<ComputeEffect>& GetBackgroundEffects() { return _backgroundEffects; }
    int& GetCurrentBackgroundEffect() { return currentBackgroundEffect; }

    // Compute gradient and sky
    VkPipeline _gradientPipeline = VK_NULL_HANDLE;
    VkPipeline _skyPipeline = VK_NULL_HANDLE;
    VkPipelineLayout _gradientPipelineLayout = VK_NULL_HANDLE;

    // Triangle
    VkPipelineLayout _trianglePipelineLayout = VK_NULL_HANDLE;
    VkPipeline _trianglePipeline = VK_NULL_HANDLE;

    // Mesh
    VkPipelineLayout _meshPipelineLayout = VK_NULL_HANDLE;
    VkPipeline _meshPipeline = VK_NULL_HANDLE;

    GPUMeshBuffers rectangle;

    void Init();
private:
    void MeshPipeline();
    void InitBackgroundPipelines();
    void ComputePipeline();
    void TrianglePipeline();

    Descriptors* _descriptors;
    DeleteQueue* _mainDeletionQueue;

    Device* _device;
    std::vector<ComputeEffect> _backgroundEffects;
    int currentBackgroundEffect = 0;
};

#endif //MELONENGINE_PIPELINE_H
