#ifndef MELONENGINE_VULKANTYPES_H
#define MELONENGINE_VULKANTYPES_H

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include <deque>
#include <functional>

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
    VkFormat imageFormat;
};

struct FrameData {
    VkCommandPool _commandPool = VK_NULL_HANDLE;
    VkCommandBuffer _commandBuffer = VK_NULL_HANDLE;

    // Sync objects
    VkSemaphore _swapchainSemaphore = VK_NULL_HANDLE;
    VkSemaphore _renderSemaphore = VK_NULL_HANDLE;
    VkFence _renderFence = VK_NULL_HANDLE;

    DeleteQueue _deletionQueue;
};

#endif //MELONENGINE_VULKANTYPES_H
