//
// Created by milos on 2026-05-11.
//

#ifndef MELONENGINE_RENDUTIL_H
#define MELONENGINE_RENDUTIL_H

#include <vulkan\vulkan.h>
namespace rutil {
    VkImageSubresourceRange ImageSubresourceRange(VkImageAspectFlags aspectMask);
    void TransitionImage(VkCommandBuffer cmdBuffer, VkImage image, VkImageLayout currentLayout, VkImageLayout newLayout);
}


#endif //MELONENGINE_RENDUTIL_H
