#ifndef MELONENGINE_BUFFER_H
#define MELONENGINE_BUFFER_H

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include "RenderTypes.h"
#include "Device.h"

class Buffer {
public:
    static AllocatedBuffer Create(size_t allocationSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage, Device* device);
    static void Destroy(VmaAllocator allocator, const AllocatedBuffer& buffer);
private:
    Device* _device;
};

#endif //MELONENGINE_BUFFER_H
