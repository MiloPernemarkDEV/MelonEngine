#include "pch.h"
#include "Device.h"
#include "RenderDefines.h"

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"
#include "RendUtil.h"
#include "Buffer.h"

Device::Device(GLFWwindow* window)
    : _window(window)
{
}

void Device::Init() {
    vkb::InstanceBuilder builder;
    auto instRet = builder.set_app_name("MelonEngine")
    .request_validation_layers(true)
    .require_api_version(1, 3, 0)
    .use_default_debug_messenger()
    .build();

    vkb::Instance vkbInst = instRet.value();

    _instance = vkbInst.instance;
    _debugMessenger = vkbInst.debug_messenger;

    VkResult surfaceResult = glfwCreateWindowSurface(_instance, _window, nullptr, &_surface);
    VK_CHECK(surfaceResult);

    VkPhysicalDeviceVulkan13Features features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES };
    features.dynamicRendering = true;
    features.synchronization2 = true;

    VkPhysicalDeviceVulkan12Features features12{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };
    features12.bufferDeviceAddress = true;
    features12.descriptorIndexing = true;

    vkb::PhysicalDeviceSelector selector{ vkbInst };
    vkb::PhysicalDevice physicalDevice = selector
    .set_minimum_version(1 ,1)
    .set_required_features_13(features)
    .set_required_features_12(features12)
    .set_surface(_surface)
    .select()
    .value();

    vkb::DeviceBuilder deviceBuilder {physicalDevice };
    vkb::Device vkbDevice = deviceBuilder.build().value();

    _device = vkbDevice.device;
    _physicalDevice = physicalDevice.physical_device;

    // the flag allows shaders to access buffers via GPU addresses, aka GPU pointers.
    VmaAllocatorCreateInfo allocatorInfo = {
        .flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
        .physicalDevice = _physicalDevice,
        .device = _device,
        .instance = _instance,
    };

    VK_CHECK(vmaCreateAllocator(&allocatorInfo, &_allocator));

    InitSwapchain();

    // some engines use 3 queues because they work in parallel
    // in this case I only use the graphics queue which can take all cmds
    _graphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
    _graphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();

    InitCommands();
    InitSyncObjects();
}

void Device::InitSwapchain() {
    vkb::SwapchainBuilder swapchainBuilder{ _physicalDevice, _device, _surface };

    _swapchainImageFormat = VK_FORMAT_B8G8R8A8_UNORM;

    vkb::Swapchain vkbSwapchain = swapchainBuilder
    .set_desired_format(VkSurfaceFormatKHR{ .format = _swapchainImageFormat, .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR })
    .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR) // cap the engines fps to the monitors refresh rate
    .add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT) // allow data to be copied onto the image
    .build()
    .value();

    _swapchain = vkbSwapchain.swapchain;
    _swapchainImages = vkbSwapchain.get_images().value();
    _swapchainImageViews = vkbSwapchain.get_image_views().value();
    _swapchainImageFormat = vkbSwapchain.image_format;
    _swapchainExtent = vkbSwapchain.extent;

    VkExtent3D drawImageExtent = {
        .width = _swapchainExtent.width,
        .height = _swapchainExtent.height,
        .depth = 1
    };

    _drawImage.imageFormat = VK_FORMAT_R16G16B16A16_SFLOAT;
    _drawImage.imageExtent = drawImageExtent;
    _drawExtent = _swapchainExtent;

    VkImageUsageFlags drawImageUsageFlags = 0;
    drawImageUsageFlags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    drawImageUsageFlags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    drawImageUsageFlags |= VK_IMAGE_USAGE_STORAGE_BIT;
    drawImageUsageFlags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    VkImageCreateInfo rimgInfo = rutil::ImageCreateInfo(_drawImage.imageFormat, drawImageUsageFlags, drawImageExtent);

    VmaAllocationCreateInfo rimgAllocInfo{};
    rimgAllocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    rimgAllocInfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    VK_CHECK(vmaCreateImage(_allocator, &rimgInfo, &rimgAllocInfo, &_drawImage.image, &_drawImage.allocation, nullptr));

    VkImageViewCreateInfo rViewInfo = rutil::ImageViewCreateInfo(_drawImage.imageFormat, _drawImage.image, VK_IMAGE_ASPECT_COLOR_BIT);

    VK_CHECK(vkCreateImageView(_device, &rViewInfo, nullptr, &_drawImage.imageView));

    _mainDeletionQueue.PushFunction([&]() {
        vkDestroyImageView(_device, _drawImage.imageView, nullptr);
        vmaDestroyImage(_allocator, _drawImage.image, _drawImage.allocation);
    });
}

void Device::CleanupSwapchain() {
    vkDestroySwapchainKHR(_device, _swapchain, nullptr);

    for (auto &swapchainImageView: _swapchainImageViews) {
        vkDestroyImageView(_device, swapchainImageView, nullptr);
    }
}

void Device::InitCommands() {

    VkCommandPoolCreateInfo poolInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = _graphicsQueueFamily,
    };

    for (auto & i : _frameData) {
        VK_CHECK(vkCreateCommandPool(_device, &poolInfo, nullptr, &i._commandPool));
        VkCommandBufferAllocateInfo commandAllocInfo = rutil::CommandBufferAllocateInfo(i._commandPool,1);
        VK_CHECK(vkAllocateCommandBuffers(_device, &commandAllocInfo, &i._commandBuffer));
    }

    VK_CHECK(vkCreateCommandPool(_device, &poolInfo, nullptr, &_uiSyncObjects._immCommandPool));
    VkCommandBufferAllocateInfo cmdAllocInfo = rutil::CommandBufferAllocateInfo(_uiSyncObjects._immCommandPool,1);
    VK_CHECK(vkAllocateCommandBuffers(_device, &cmdAllocInfo, &_uiSyncObjects._immCommandBuffer));

    _mainDeletionQueue.PushFunction([=]() {
        vkDestroyCommandPool(_device, _uiSyncObjects._immCommandPool, nullptr);
    });
}

GPUMeshBuffers Device::UploadMesh(std::span<uint32_t> indices, std::span<Vertex> vertices) {
    const size_t vertexBufferSize = vertices.size() * sizeof(Vertex);
    const size_t indexBufferSize = indices.size() * sizeof(uint32_t);

    GPUMeshBuffers newSurface;

    newSurface.vertexBuffer = Buffer::Create(
        vertexBufferSize,
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT |VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
        VMA_MEMORY_USAGE_GPU_ONLY
    );
    // get device address
    VkBufferDeviceAddressInfo deviceAdressInfo{
        .sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
        .buffer = newSurface.vertexBuffer.buffer
    };

    // store device address
    newSurface.vertexBufferAddress = vkGetBufferDeviceAddress(_device, &deviceAdressInfo);

    newSurface.indexBuffer = Buffer::Create(
        indexBufferSize,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VMA_MEMORY_USAGE_GPU_ONLY
    );
}

// When a command pool is destroyed all cmd buffers allocated from it are freed
void Device::CleanupCommandPool() {
    for (auto & i : _frameData) {
        vkDestroyCommandPool(_device, i._commandPool, nullptr);
    }
}

void Device::InitSyncObjects() {
    VkFenceCreateInfo fenceInfo{
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT
    };

    VkSemaphoreCreateInfo semaphoreInfo{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0
    };

    for (auto & i : _frameData) {
        VK_CHECK(vkCreateFence(_device, &fenceInfo, nullptr, &i._renderFence));

        VK_CHECK(vkCreateSemaphore(_device, &semaphoreInfo, nullptr, &i._swapchainSemaphore));
        VK_CHECK(vkCreateSemaphore(_device, &semaphoreInfo, nullptr, &i._renderSemaphore));
    }

    VK_CHECK(vkCreateFence(_device, &fenceInfo, nullptr, &_uiSyncObjects._immFence));
    _mainDeletionQueue.PushFunction([=]() {
        vkDestroyFence(_device, _uiSyncObjects._immFence, nullptr);
    });
}

void Device::Cleanup() {
    vkDeviceWaitIdle(_device);

    for (int i = 0; i < FRAMES_IN_FLIGHT; i++) {

        //destroy sync objects
        vkDestroyFence(_device, _frameData[i]._renderFence, nullptr);
        vkDestroySemaphore(_device, _frameData[i]._renderSemaphore, nullptr);
        vkDestroySemaphore(_device ,_frameData[i]._swapchainSemaphore, nullptr);

        _frameData[i]._deletionQueue.Flush();
    }

    CleanupCommandPool();

    CleanupSwapchain();
    _mainDeletionQueue.Flush();

    vkDestroyDevice(_device, nullptr);
    vkDestroySurfaceKHR(_instance, _surface, nullptr);
    vkb::destroy_debug_utils_messenger(_instance, _debugMessenger);
    vkDestroyInstance(_instance, nullptr);
}
