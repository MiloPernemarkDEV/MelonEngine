#ifndef MELONENGINE_RENDUTIL_H
#define MELONENGINE_RENDUTIL_H

#include <vulkan\vulkan.h>
namespace rutil {
    VkImageSubresourceRange ImageSubresourceRange(VkImageAspectFlags aspectMask);

    void TransitionImage(VkCommandBuffer cmdBuffer, VkImage image, VkImageLayout currentLayout, VkImageLayout newLayout);

    VkCommandBufferBeginInfo CommandBufferBeginInfo(VkCommandBufferUsageFlags flags);

    VkSemaphoreSubmitInfo SemaphoreSubmitInfo(VkPipelineStageFlags2 stageMask, VkSemaphore semaphore);

    VkCommandBufferSubmitInfo CommandBufferSubmitInfo(VkCommandBuffer cmd);

    VkSubmitInfo2 SubmitInfo(VkCommandBufferSubmitInfo* cmd, VkSemaphoreSubmitInfo* signalSemaphoreInfo,

    VkSemaphoreSubmitInfo* waitSemaphoreInfo);
}


#endif //MELONENGINE_RENDUTIL_H
