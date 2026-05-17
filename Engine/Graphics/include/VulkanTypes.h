#ifndef MELONENGINE_VULKANTYPES_H
#define MELONENGINE_VULKANTYPES_H

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include <deque>
#include <functional>
#include <span>
#include "math/Vec4f.h"

struct DeleteQueue {
    std::deque<std::function<void()>> deletors;

    void PushFunction(std::function<void()>&& function) {
        deletors.push_back(function);
    }

    void Flush() {
        for (auto& deletor : deletors) {
            deletor();
        }
        deletors.clear();
    }
};

struct AllocatedImage {
    VkImage image = VK_NULL_HANDLE;
    VkImageView imageView = VK_NULL_HANDLE;
    VmaAllocation allocation = VK_NULL_HANDLE;
    VkExtent3D imageExtent;
    VkFormat imageFormat = VK_FORMAT_UNDEFINED;
};

struct FrameData {
    VkCommandPool _commandPool = VK_NULL_HANDLE;
    VkCommandBuffer _commandBuffer = VK_NULL_HANDLE;

    VkSemaphore _swapchainSemaphore = VK_NULL_HANDLE;
    VkSemaphore _renderSemaphore = VK_NULL_HANDLE;
    VkFence _renderFence = VK_NULL_HANDLE;

    DeleteQueue _deletionQueue;
};

struct MelonImguiObjects {
    VkFence _immFence = VK_NULL_HANDLE;
    VkCommandBuffer _immCommandBuffer = VK_NULL_HANDLE;
    VkCommandPool _immCommandPool = VK_NULL_HANDLE;
};

struct ComputePushConstants {
    Vec4f data1;
    Vec4f data2;
    Vec4f data3;
    Vec4f data4;
};

struct ComputeEffect {
    const char* name = nullptr;
    VkPipeline pipeline = VK_NULL_HANDLE;
    VkPipelineLayout layout = VK_NULL_HANDLE;
    ComputePushConstants data;
};

struct AllocatedBuffer {
    VkBuffer buffer = VK_NULL_HANDLE;
    VmaAllocation allocation = VK_NULL_HANDLE;
    VmaAllocationInfo info{};
};

#endif //MELONENGINE_VULKANTYPES_H
