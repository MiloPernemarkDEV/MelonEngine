#include "Buffer.h"
#include "RenderDefines.h"

AllocatedBuffer Buffer::Create(size_t allocationSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage, Device* device) {
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.pNext = nullptr;
    bufferInfo.size = allocationSize;
    bufferInfo.usage = usage;

    VmaAllocationCreateInfo vmaAllocInfo = {
        .flags = VMA_ALLOCATION_CREATE_MAPPED_BIT,
        .usage = memoryUsage,
    };

    AllocatedBuffer newBuffer{};

    VK_CHECK(vmaCreateBuffer(
        device->_allocator,
        &bufferInfo,
        &vmaAllocInfo,
        &newBuffer.buffer,
        &newBuffer.allocation,
        &newBuffer.info)
    );

    return newBuffer;
}

void Buffer::Destroy(VmaAllocator allocator, const AllocatedBuffer &buffer) {
    vmaDestroyBuffer(allocator, buffer.buffer, buffer.allocation);
}


