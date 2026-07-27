#ifndef MELON_VK_DEVICE_H
#define MELON_VK_DEVICE_H

#include <vulkan/vulkan.h>

namespace DeviceManager {
    VkPhysicalDevice get_physical_device(VkInstance instance, VkQueueFlags required_queues);
    VkDevice create_device(VkPhysicalDevice physicalDevice);
    bool is_valid(VkQueueFlagBits flags);
    void destroy(VkDevice device);
}

#endif //MELON_VK_DEVICE_H
