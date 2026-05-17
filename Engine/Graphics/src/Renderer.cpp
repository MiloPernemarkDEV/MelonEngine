#include "pch.h"
#include "Renderer.h"
#include "RenderDefines.h"
#include "RendUtil.h"
#include "math/Vec4f.h"

Renderer::Renderer(GLFWwindow* window)
    : _window(window),
      _device(window),
      _descriptors(&_device),
      _pipelines(&_descriptors, &_device._mainDeletionQueue, &_device),
      _melonImgui(&_device, window, &_pipelines)
{

}

bool Renderer::Init() {
    _device.Init();
    _descriptors.Init();
    _pipelines.Init();
    _melonImgui.Init();

    _device._mainDeletionQueue.PushFunction([&]() {
        vmaDestroyAllocator(_device._allocator);
    } );
    return true;
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

    frame._deletionQueue.Flush();

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

    // initialize draw image once
    if (_device.GetFrameNumber() == 0)
    {
        rutil::TransitionImage(
            cmd,
            _device._drawImage.image,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_GENERAL
        );
    }

    // draw background
    DrawBackground(cmd);

    // geometry pass
    rutil::TransitionImage(
        cmd,
        _device._drawImage.image,
        VK_IMAGE_LAYOUT_GENERAL,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    );

    DrawGeometry(cmd);

    // prepare for copy
    rutil::TransitionImage(
        cmd,
        _device._drawImage.image,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
    );

    rutil::TransitionImage(
        cmd,
        _device.GetSwapchainImages()[swapchainImageIndex],
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
    );

    // copy draw image -> swapchain
    rutil::CopyImageToImage(
        cmd,
        _device._drawImage.image,
        _device.GetSwapchainImages()[swapchainImageIndex],
        _device._drawExtent,
        _device._swapchainExtent
    );

    // prepare swapchain image for imgui rendering
    rutil::TransitionImage(
        cmd,
        _device.GetSwapchainImages()[swapchainImageIndex],
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    );

    // draw imgui on top
    _melonImgui.Draw(
        cmd,
        _device._swapchainImageViews[swapchainImageIndex]
    );

    // prepare for presentation
    rutil::TransitionImage(
        cmd,
        _device.GetSwapchainImages()[swapchainImageIndex],
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    );

    // reset draw image for next frame
    rutil::TransitionImage(
        cmd,
        _device._drawImage.image,
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        VK_IMAGE_LAYOUT_GENERAL
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
        rutil::SubmitInfo(
            &cmdInfo,
            &signalInfo,
            &waitInfo
        );

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
}

void Renderer::Terminate() {
    _device.Cleanup();
}

void Renderer::DrawDebugUI() {
    _melonImgui.DrawDebugUI();
}

void Renderer::DrawBackground(VkCommandBuffer cmd) {
    ComputeEffect& effect = _pipelines.GetBackgroundEffects()[_pipelines.GetCurrentBackgroundEffect()];

    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, effect.pipeline);
    vkCmdBindDescriptorSets(
        cmd,
        VK_PIPELINE_BIND_POINT_COMPUTE,
        _pipelines._gradientPipelineLayout,
        0,
        1,
        &_descriptors._drawImageDescriptors,
        0,
        nullptr
    );

    vkCmdPushConstants(cmd, _pipelines._gradientPipelineLayout, VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof(ComputePushConstants), &effect.data);

    vkCmdDispatch(
        cmd,
        std::ceil(_device._drawExtent.width / 16.0),
        std::ceil(_device._drawExtent.height / 16.0),
        1
    );
}

void Renderer::DrawGeometry(VkCommandBuffer cmd) {
    VkRenderingAttachmentInfo colorAttachment = rutil::RenderingAttachmentInfo(_device._drawImage.imageView, nullptr, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    VkRenderingInfo renderingInfo = rutil::RenderingInfo(_device._drawExtent, &colorAttachment, nullptr);
    vkCmdBeginRendering(cmd, &renderingInfo);

    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelines._trianglePipeline);

    VkViewport viewport;
    viewport.x = 0;
    viewport.y = 0;
    viewport.width = _device._drawExtent.width;
    viewport.height = _device._drawExtent.height;
    viewport.minDepth = 0.f;
    viewport.maxDepth = 1.f;

    vkCmdSetViewport(cmd, 0, 1, &viewport);

    VkRect2D scissor = {};
    scissor.offset.x = 0;
    scissor.offset.y = 0;
    scissor.extent.width = _device._drawExtent.width;
    scissor.extent.height = _device._drawExtent.height;

    vkCmdSetScissor(cmd, 0, 1, &scissor);
    vkCmdDraw(cmd, 3, 1, 0, 0);
    vkCmdEndRendering(cmd);
}
