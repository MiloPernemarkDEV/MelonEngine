#ifndef MELONENGINE_VULKAN_TYPES_H
#define MELONENGINE_VULKAN_TYPES_H

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include <deque>
#include <functional>
#include "math/vec4f.h"
#include "math/vec3f.h"
#include "math/mat4.h"

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
    VkFence _renderFence = VK_NULL_HANDLE;

    DeleteQueue _deletionQueue;
};

struct MelonImguiObjects {
    VkFence _immFence = VK_NULL_HANDLE;
    VkCommandBuffer _immCommandBuffer = VK_NULL_HANDLE;
    VkCommandPool _immCommandPool = VK_NULL_HANDLE;
};

struct ComputePushConstants {
    vec4f data1;
    vec4f data2;
    vec4f data3;
    vec4f data4;
};

struct ComputeEffect {
    const char* name = nullptr;
    VkPipeline pipeline = VK_NULL_HANDLE;
    VkPipelineLayout layout = VK_NULL_HANDLE;
    ComputePushConstants data;
};

struct AllocatedBuffer {
    VkBuffer buffer = VK_NULL_HANDLE;
    VmaAllocation allocation;
    VmaAllocationInfo info;
};

struct Vertex {
    vec3f position;
    float uvX;
    vec3f normal;
    float uvY;
    vec4f color;
};

struct GPUMeshBuffers {
    AllocatedBuffer indexBuffer;
    AllocatedBuffer vertexBuffer;
    // The device address is like a GPU pointer to an
    // exact address in the gpus virtual address space
    VkDeviceAddress vertexBufferAddress;
};

struct GPUDrawPushConstants {
    mat4 worldMatrix{};
    VkDeviceAddress vertexBuffer;
};

#endif //MELONENGINE_VULKAN_TYPES_H
