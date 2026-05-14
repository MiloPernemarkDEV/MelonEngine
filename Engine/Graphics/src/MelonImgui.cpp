#include "pch.h"
#include "MelonImgui.h"
#include "Device.h"
#include "RendUtil.h"
#include "RenderDefines.h"

MelonImgui::MelonImgui(Device* device, GLFWwindow* window)
    : _device(device), _window(window)
{
    _uiSyncObjects = &_device->_uiSyncObjects;
}

void MelonImgui::Init() {
    VkDescriptorPoolSize poolSizes[] = { { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
    };

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    poolInfo.maxSets = 1000;
    poolInfo.poolSizeCount = (uint32_t)std::size(poolSizes);
    poolInfo.pPoolSizes = poolSizes;

    VkDescriptorPool imguiPool;
    VK_CHECK(vkCreateDescriptorPool(_device->GetDevice(), &poolInfo, nullptr, &imguiPool));

    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForVulkan(_window, true);
    ImGui_ImplVulkan_InitInfo initInfo = {};
    initInfo.Instance = _device->GetInstance();
    initInfo.PhysicalDevice = _device->_physicalDevice;
    initInfo.Device = _device->GetDevice();
    initInfo.Queue = _device->GetGraphicsQueue();
    initInfo.DescriptorPool = imguiPool;
    initInfo.MinImageCount = 3;
    initInfo.ImageCount = 3;
    initInfo.UseDynamicRendering = true;

    initInfo.PipelineRenderingCreateInfo = {.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO};
    initInfo.PipelineRenderingCreateInfo.colorAttachmentCount = 1;
    initInfo.PipelineRenderingCreateInfo.pColorAttachmentFormats = &_device->GetFormat();

    initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

    ImGui_ImplVulkan_Init(&initInfo);
    ImGui_ImplVulkan_CreateFontsTexture();

    _device->_mainDeletionQueue.PushFunction([this, imguiPool]() {
        ImGui_ImplVulkan_Shutdown();
        vkDestroyDescriptorPool(_device->GetDevice(), imguiPool, nullptr);
    });
}

void MelonImgui::ImmediateSubmit(std::function<void(VkCommandBuffer cmd)> func) {
    VK_CHECK(vkResetFences(_device->GetDevice(), 1, &_uiSyncObjects->_immFence));
    VK_CHECK(vkResetCommandBuffer(_uiSyncObjects->_immCommandBuffer, 0));

    VkCommandBuffer cmd = _uiSyncObjects->_immCommandBuffer;

    VkCommandBufferBeginInfo cmdBeginInfo = rutil::CommandBufferBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    VK_CHECK(vkBeginCommandBuffer(cmd, &cmdBeginInfo));
    func(cmd);

    VK_CHECK(vkEndCommandBuffer(cmd));
    VkCommandBufferSubmitInfo cmdinfo = rutil::CommandBufferSubmitInfo(cmd);
    VkSubmitInfo2 submit = rutil::SubmitInfo(&cmdinfo, nullptr, nullptr);

    VK_CHECK(vkQueueSubmit2(_device->GetGraphicsQueue(), 1, &submit, _uiSyncObjects->_immFence));
    VK_CHECK(vkWaitForFences(_device->GetDevice(), 1, &_uiSyncObjects->_immFence, true, 9999999999));

}

void MelonImgui::Draw(VkCommandBuffer cmd, VkImageView targetImageView) {
    VkRenderingAttachmentInfo colorAttachment = rutil::RenderingAttachmentInfo(targetImageView, nullptr, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    VkRenderingInfo renderInfo = rutil::RenderingInfo(_device->_swapchainExtent, &colorAttachment, nullptr);

    vkCmdBeginRendering(cmd, &renderInfo);

    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);

    vkCmdEndRendering(cmd);
}
