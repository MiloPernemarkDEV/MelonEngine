#include "Renderer.h"
#include "RenderDefines.h"
#include "RendUtil.h"
#include <cmath>

Renderer::Renderer(GLFWwindow *window)
    : _window(window), _device(window) {
}

void Renderer::Draw() {
    VK_CHECK(vkWaitForFences(_device.GetDevice(), 1, &_device.GetCurrentFrame()._renderFence, true, 1000000000));

    uint32_t swapchainImageIndex;
    VK_CHECK(vkAcquireNextImageKHR(_device.GetDevice(), _device.GetSwapchain(), 1000000000,
        _device.GetCurrentFrame()._swapchainSemaphore, nullptr, &swapchainImageIndex));

    VkCommandBuffer cmd = _device.GetCurrentFrame()._commandBuffer;

    VK_CHECK(vkResetCommandBuffer(_device.GetCurrentFrame()._commandBuffer, 0));

    VkCommandBufferBeginInfo beginInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    };

    // start recording
    VK_CHECK(vkBeginCommandBuffer(cmd, &beginInfo));

    rutil::TransitionImage(cmd, _device.GetSwapchainImages()[swapchainImageIndex],
        VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

    VkClearColorValue clearValue;
    float flash = std::abs(std::sin(static_cast<float>(_device.GetFrameNumber()) / 120.f));
    clearValue = { { 0.0f, 0.0f, flash, 1.0f } };

    VkImageSubresourceRange clearRange = rutil::ImageSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT);
    vkCmdClearColorImage(cmd, _device.GetSwapchainImages()[swapchainImageIndex], VK_IMAGE_LAYOUT_GENERAL, &clearValue, 1, &clearRange);
    rutil::TransitionImage(cmd, _device.GetSwapchainImages()[swapchainImageIndex], VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

    // end recording
    VK_CHECK(vkEndCommandBuffer(cmd));

    VK_CHECK(vkResetFences(_device.GetDevice(), 1, &_device.GetCurrentFrame()._renderFence));
}

bool Renderer::Init() {
    _device.Init();

    return true;
}

void Renderer::Terminate() {
    _device.Cleanup();
}