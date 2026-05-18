//
// Created by milos on 2026-05-14.
//

#ifndef MELONENGINE_DESCRIPTORS_H
#define MELONENGINE_DESCRIPTORS_H

#include <vector>
#include <vulkan/vulkan_core.h>
#include <span>

class Device;

struct DescriptorLayoutBuilder {
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    void AddBinding(uint32_t binding, VkDescriptorType type);
    void Clear();
    VkDescriptorSetLayout Build(VkDevice device, VkShaderStageFlags shaderStages, void* pNext = nullptr,
        VkDescriptorSetLayoutCreateFlags flags = 0);
};

struct DescriptorAllocator {
    struct PoolSizeRatio {
        VkDescriptorType type;
        float ratio;
    };
    VkDescriptorPool pool = VK_NULL_HANDLE;
    void InitPool(VkDevice device, uint32_t maxSets, std::span<PoolSizeRatio> poolRatios);
    void ClearDescriptors(VkDevice device);
    void DestroyPool(VkDevice device);
    VkDescriptorSet Allocate(VkDevice device, VkDescriptorSetLayout layout);
};

class Descriptors {
public:
    Descriptors(Device* device);
    void Init();

    DescriptorAllocator globalDescriptorAllocator;
    VkDescriptorSet _drawImageDescriptors;
    VkDescriptorSetLayout _drawImageDescriptorLayout;
private:
    Device* _device;
};

#endif //MELONENGINE_DESCRIPTORS_H
