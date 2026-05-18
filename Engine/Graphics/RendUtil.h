#ifndef MELONENGINE_REND_UTIL_H
#define MELONENGINE_REND_UTIL_H

#include <vulkan\vulkan.h>
#include <Device.h>
#include <functional>

namespace rutil {
    // Images
    VkImageSubresourceRange ImageSubresourceRange(VkImageAspectFlags aspectMask);
    void TransitionImage(VkCommandBuffer cmdBuffer, VkImage image, VkImageLayout currentLayout, VkImageLayout newLayout);
    VkImageCreateInfo ImageCreateInfo(VkFormat format, VkImageUsageFlags usageFlags, VkExtent3D extent);
    VkImageViewCreateInfo ImageViewCreateInfo(VkFormat format, VkImage image, VkImageAspectFlags aspectFlags);
    void CopyImageToImage(VkCommandBuffer cmd, VkImage source, VkImage destination, VkExtent2D srcSize, VkExtent2D dstSize);

    VkCommandBufferBeginInfo CommandBufferBeginInfo(VkCommandBufferUsageFlags flags);
    VkSemaphoreSubmitInfo SemaphoreSubmitInfo(VkPipelineStageFlags2 stageMask, VkSemaphore semaphore);
    VkCommandBufferSubmitInfo CommandBufferSubmitInfo(VkCommandBuffer cmd);
    VkCommandBufferAllocateInfo CommandBufferAllocateInfo(VkCommandPool pool, uint32_t count);

    VkSubmitInfo2 SubmitInfo(VkCommandBufferSubmitInfo* cmd, VkSemaphoreSubmitInfo* signalSemaphoreInfo,
    VkSemaphoreSubmitInfo* waitSemaphoreInfo);
    VkRenderingAttachmentInfo RenderingAttachmentInfo(VkImageView view, VkClearValue* clear, VkImageLayout layout);

    VkRenderingInfo RenderingInfo(VkExtent2D renderExtent, VkRenderingAttachmentInfo* colorAttachment,
    VkRenderingAttachmentInfo* depthAttachment);
    VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo(VkShaderStageFlagBits flags, VkShaderModule shaderModule);


    VkPipelineLayoutCreateInfo PipelineLayoutCreateInfo();
    void GlobalImmediateSubmit(std::function<void(VkCommandBuffer cmd)> func);


    bool LoadShaderModule(const char* filename, VkDevice device,
    VkShaderModule* outShaderModule);
}


#endif //MELONENGINE_REND_UTIL_H
