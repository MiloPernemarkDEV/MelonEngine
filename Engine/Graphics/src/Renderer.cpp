#include "pch.h"
#include "Renderer.h"
#include "RenderDefines.h"
#include "RendUtil.h"


Renderer::Renderer(GLFWwindow *window)
    : _window(window), _device(window) {
}

void Renderer::Draw()
{
    FrameData& frame = _device.GetCurrentFrame();

    VK_CHECK(vkWaitForFences(
        _device.GetDevice(),
        1,
        &frame._renderFence,
        true,
        1000000000
    ));

    VK_CHECK(vkResetFences(
        _device.GetDevice(),
        1,
        &frame._renderFence
    ));

    uint32_t swapchainImageIndex;

    VK_CHECK(vkAcquireNextImageKHR(
        _device.GetDevice(),
        _device.GetSwapchain(),
        1000000000,
        frame._swapchainSemaphore,
        nullptr,
        &swapchainImageIndex
    ));

    VK_CHECK(vkResetCommandBuffer(frame._commandBuffer, 0));

    VkCommandBuffer cmd = frame._commandBuffer;

    VkCommandBufferBeginInfo beginInfo =
        rutil::CommandBufferBeginInfo(
            VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
        );

    VK_CHECK(vkBeginCommandBuffer(cmd, &beginInfo));

    rutil::TransitionImage(
        cmd,
        _device.GetSwapchainImages()[swapchainImageIndex],
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        VK_IMAGE_LAYOUT_GENERAL
    );

    VkClearColorValue clearValue;
    float flash = std::abs(std::sin(
        static_cast<float>(_device.GetFrameNumber()) / 120.f));

    clearValue = {{0.0f, 0.0f, flash, 1.0f}};

    VkImageSubresourceRange clearRange =
        rutil::ImageSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT);

    vkCmdClearColorImage(
        cmd,
        _device.GetSwapchainImages()[swapchainImageIndex],
        VK_IMAGE_LAYOUT_GENERAL,
        &clearValue,
        1,
        &clearRange
    );

    rutil::TransitionImage(
        cmd,
        _device.GetSwapchainImages()[swapchainImageIndex],
        VK_IMAGE_LAYOUT_GENERAL,
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    );

    VK_CHECK(vkEndCommandBuffer(cmd));

    VkCommandBufferSubmitInfo cmdInfo =
        rutil::CommandBufferSubmitInfo(cmd);

    VkSemaphoreSubmitInfo waitInfo =
        rutil::SemaphoreSubmitInfo(
            VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
            frame._swapchainSemaphore
        );

    VkSemaphoreSubmitInfo signalInfo =
        rutil::SemaphoreSubmitInfo(
            VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT,
            frame._renderSemaphore
        );

    VkSubmitInfo2 submit =
        rutil::SubmitInfo(&cmdInfo, &signalInfo, &waitInfo);

    VK_CHECK(vkQueueSubmit2(
        _device.GetGraphicsQueue(),
        1,
        &submit,
        frame._renderFence
    ));

    VkPresentInfoKHR presentInfo{
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &frame._renderSemaphore,
        .swapchainCount = 1,
        .pSwapchains = &_device.GetSwapchain(),
        .pImageIndices = &swapchainImageIndex,
    };

    VK_CHECK(vkQueuePresentKHR(
        _device.GetGraphicsQueue(),
        &presentInfo
    ));

    _device.IncrementFrameNumber();
}

bool Renderer::Init() {
    _device.Init();

    return true;
}

void Renderer::Terminate() {
    _device.Cleanup();
}