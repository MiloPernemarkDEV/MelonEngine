#include "pch.h"
#include "MelonImGui.h"
#include "Device.h"
#include "RendUtil.h"
#include "RenderDefines.h"

MelonImGui::MelonImGui(Device* device, GLFWwindow* window, Pipelines* pipelines)
    : _device(device), _window(window), _pipelines(pipelines)
{
    _uiSyncObjects = &_device->_uiSyncObjects;
}

void MelonImGui::Init() {
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

    SetImGuiStyleConfig();
}

void MelonImGui::ImmediateSubmit(std::function<void(VkCommandBuffer cmd)> func) {
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

void MelonImGui::Draw(VkCommandBuffer cmd, VkImageView targetImageView) {
    VkRenderingAttachmentInfo colorAttachment = rutil::RenderingAttachmentInfo(targetImageView, nullptr, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

    VkRenderingInfo renderInfo = rutil::RenderingInfo(_device->_swapchainExtent, &colorAttachment, nullptr);
    vkCmdBeginRendering(cmd, &renderInfo);
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);
    vkCmdEndRendering(cmd);
}

void MelonImGui::DrawDebugUI() {
    if (ImGui::Begin("Compute Shaders"))
    {
        auto& effects = _pipelines->GetBackgroundEffects();
        int& index = _pipelines->GetCurrentBackgroundEffect();

        ComputeEffect& selected = effects[index];

        ImGui::TextUnformatted("Selected Effect:");
        ImGui::SameLine();
        ImGui::TextUnformatted(selected.name);

        ImGui::Separator();

        ImGui::SliderInt("Effect Index", &index, 0, (int)effects.size() - 1);

        ImGui::Spacing();
        ImGui::SeparatorText("Parameters");

        ImGui::DragFloat4("Data 1", (float*)&selected.data.data1, 0.01f);
    }
    ImGui::End();
}

void MelonImGui::SetImGuiStyleConfig()
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.Alpha = 1.0f;
    style.FrameRounding = 3.0f;

    style.Colors[ImGuiCol_Text]                 = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled]         = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_WindowBg]             = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
    style.Colors[ImGuiCol_PopupBg]              = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);

    style.Colors[ImGuiCol_Border]               = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
    style.Colors[ImGuiCol_BorderShadow]         = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);

    style.Colors[ImGuiCol_FrameBg]              = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
    style.Colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive]        = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);

    style.Colors[ImGuiCol_TitleBg]              = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
    style.Colors[ImGuiCol_TitleBgActive]        = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);

    style.Colors[ImGuiCol_MenuBarBg]            = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);

    style.Colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);

    style.Colors[ImGuiCol_CheckMark]            = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);

    style.Colors[ImGuiCol_SliderGrab]           = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive]     = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);

    style.Colors[ImGuiCol_Button]               = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    style.Colors[ImGuiCol_ButtonHovered]        = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive]         = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);

    style.Colors[ImGuiCol_Header]               = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
    style.Colors[ImGuiCol_HeaderHovered]        = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    style.Colors[ImGuiCol_HeaderActive]         = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);

    style.Colors[ImGuiCol_ResizeGrip]          = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
    style.Colors[ImGuiCol_ResizeGripHovered]   = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive]    = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);

    style.Colors[ImGuiCol_TextSelectedBg]      = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);

    if (bStyleDark_)
    {
        for (int i = 0; i < ImGuiCol_COUNT; i++)
        {
            ImVec4& col = style.Colors[i];

            float H, S, V;
            ImGui::ColorConvertRGBtoHSV(col.x, col.y, col.z, H, S, V);

            if (S < 0.1f)
                V = 1.0f - V;

            ImGui::ColorConvertHSVtoRGB(H, S, V, col.x, col.y, col.z);

            if (col.w < 1.0f)
                col.w *= _alpha;
        }
    }
    else
    {
        for (int i = 0; i < ImGuiCol_COUNT; i++)
        {
            ImVec4& col = style.Colors[i];

            if (col.w < 1.0f)
            {
                col.x *= _alpha;
                col.y *= _alpha;
                col.z *= _alpha;
                col.w *= _alpha;
            }
        }
    }
}

