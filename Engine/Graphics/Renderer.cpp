#include "pch.h"
#include "Renderer.h"
#include "RenderDefines.h"
#include "RendUtil.h"
#include "TilemapEditorUi.h"

Renderer::Renderer(GLFWwindow* window)
    : _window(window),
      _device(window),
      _descriptors(&_device),
      _pipelines(&_descriptors, &_device._mainDeletionQueue, &_device),
      _melonImgui(&_device, window, &_pipelines)
{
}

bool Renderer::init() {
    _device._melonImgui = &_melonImgui;
    _device.Init();
    _descriptors.Init();
    _pipelines.Init();
    _melonImgui.Init();
    _device.InitMeshData();
    _pipelines.rectangle = _device.rectangle;

    _device._mainDeletionQueue.PushFunction([&]() {
        vmaDestroyAllocator(_device._allocator);
    });
    return true;
}

void Renderer::draw_frame()
{
    FrameData& frame = _device.GetCurrentFrame();

    VK_CHECK(vkWaitForFences(
        _device.GetDevice(),
        1,
        &frame._renderFence,
        VK_TRUE,
        UINT64_MAX
    ));

    frame._deletionQueue.Flush();

    VK_CHECK(vkResetFences(
        _device.GetDevice(),
        1,
        &frame._renderFence
    ));

    uint32_t swapchainImageIndex = 0;

    VK_CHECK(vkAcquireNextImageKHR(
        _device.GetDevice(),
        _device.GetSwapchain(),
        UINT64_MAX,
        frame._swapchainSemaphore,
        VK_NULL_HANDLE,
        &swapchainImageIndex
    ));

    VkSemaphore& renderSemaphore = _device._renderSemaphores[swapchainImageIndex];

    VK_CHECK(vkResetCommandBuffer(frame._commandBuffer, 0));

    VkCommandBuffer cmd = frame._commandBuffer;

    VkCommandBufferBeginInfo beginInfo =
        rutil::CommandBufferBeginInfo(
            VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
        );

    VK_CHECK(vkBeginCommandBuffer(cmd, &beginInfo));

    if (_device.GetFrameNumber() == 0) {
        rutil::TransitionImage(cmd, _device._drawImage.image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL);
    }

    DrawBackground(cmd);

    rutil::TransitionImage(cmd, _device._drawImage.image, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    DrawGeometry(cmd);
    _melonImgui.Draw(cmd, _device._drawImage.imageView);

    rutil::TransitionImage(cmd, _device._drawImage.image, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
    rutil::TransitionImage(cmd, _device.GetSwapchainImages()[swapchainImageIndex], VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    rutil::CopyImageToImage(cmd, _device._drawImage.image, _device.GetSwapchainImages()[swapchainImageIndex], _device._drawExtent, _device._swapchainExtent);

    rutil::TransitionImage(cmd, _device.GetSwapchainImages()[swapchainImageIndex], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
    rutil::TransitionImage(cmd, _device._drawImage.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL);

    VK_CHECK(vkEndCommandBuffer(cmd));

    VkCommandBufferSubmitInfo cmdInfo = rutil::CommandBufferSubmitInfo(cmd);

    VkSemaphoreSubmitInfo waitInfo = rutil::SemaphoreSubmitInfo(
        VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
        frame._swapchainSemaphore
    );

    VkSemaphoreSubmitInfo signalInfo = rutil::SemaphoreSubmitInfo(
        VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
        renderSemaphore
    );

    VkSubmitInfo2 submit = rutil::SubmitInfo(&cmdInfo, &signalInfo, &waitInfo);

    VK_CHECK(vkQueueSubmit2(
        _device.GetGraphicsQueue(),
        1,
        &submit,
        frame._renderFence
    ));

    VkPresentInfoKHR presentInfo{
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &renderSemaphore,
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

void Renderer::terminate() {
    _device.Cleanup();
}

void Renderer::draw_editor_windows() {
    _melonImgui.draw_debug_ui("Compute Backgrounds");
    _melonImgui.draw_top_menu();
    ui::draw_tilemap_editor_ui("Tilemap Editor");
}

void Renderer::DrawBackground(VkCommandBuffer cmd) {
    ComputeEffect& effect = _pipelines.GetBackgroundEffects()[_pipelines.GetCurrentBackgroundEffect()];

    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, effect.pipeline);
    vkCmdBindDescriptorSets(
        cmd,
        VK_PIPELINE_BIND_POINT_COMPUTE,
        effect.layout,
        0,
        1,
        &_descriptors._drawImageDescriptors,
        0,
        nullptr
    );
    vkCmdPushConstants(
        cmd,
        effect.layout,
        VK_SHADER_STAGE_COMPUTE_BIT,
        0,
        sizeof(ComputePushConstants),
        &effect.data
    );

    const uint32_t groupX = (_device._drawExtent.width + 15) / 16;
    const uint32_t groupY = (_device._drawExtent.height + 15) / 16;
    vkCmdDispatch(cmd, groupX, groupY, 1);
}

void Renderer::DrawGeometry(VkCommandBuffer cmd) {
    const GPUMeshBuffers& mesh = _device.rectangle;
    if (mesh.indexBuffer.buffer == VK_NULL_HANDLE || mesh.vertexBufferAddress == 0) {
        return;
    }

    VkRenderingAttachmentInfo colorAttachment = rutil::RenderingAttachmentInfo(
        _device._drawImage.imageView,
        nullptr,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    );
    VkRenderingInfo renderInfo = rutil::RenderingInfo(_device._drawExtent, &colorAttachment, nullptr);
    vkCmdBeginRendering(cmd, &renderInfo);

    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelines._meshPipeline);

    VkViewport viewport{
        .x = 0.0f,
        .y = 0.0f,
        .width = static_cast<float>(_device._drawExtent.width),
        .height = static_cast<float>(_device._drawExtent.height),
        .minDepth = 0.0f,
        .maxDepth = 1.0f,
    };
    vkCmdSetViewport(cmd, 0, 1, &viewport);

    VkRect2D scissor{
        .offset = {0, 0},
        .extent = _device._drawExtent,
    };
    vkCmdSetScissor(cmd, 0, 1, &scissor);

    GPUDrawPushConstants pushConstants{};
    pushConstants.worldMatrix = mat4{};
    pushConstants.vertexBuffer = mesh.vertexBufferAddress;
    vkCmdPushConstants(
        cmd,
        _pipelines._meshPipelineLayout,
        VK_SHADER_STAGE_VERTEX_BIT,
        0,
        sizeof(GPUDrawPushConstants),
        &pushConstants
    );

    vkCmdBindIndexBuffer(cmd, mesh.indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(cmd, 6, 1, 0, 0, 0);

    vkCmdEndRendering(cmd);
}
