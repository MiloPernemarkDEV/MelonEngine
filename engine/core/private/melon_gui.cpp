#include "melon_gui.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_vulkan.h>

#include "vk_renderer.h"
#include "logger.h"

#ifdef _WIN32
#include <imgui_impl_win32.h>

namespace MelonGUI {
    void Init(HWND windowHandle, const GuiInitInfo* guiInitInfo) {
        if (!guiInitInfo) {
            ME_LOG(Error, "GuiInitInfo argument is nullptr");
            return;
        }

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui_ImplVulkan_InitInfo vk_info = {};
        vk_info.ApiVersion = guiInitInfo->ApiVersion;
        vk_info.Instance = guiInitInfo->Instance;
        vk_info.PhysicalDevice = guiInitInfo->PhysicalDevice;
        vk_info.Device = guiInitInfo->Device;
        vk_info.QueueFamily = guiInitInfo->QueueFamily;
        vk_info.Queue = guiInitInfo->Queue;
        vk_info.DescriptorPool = guiInitInfo->DescriptorPool;
        vk_info.DescriptorPoolSize = guiInitInfo->DescriptorPoolSize; // Might be handled internally by imgui
        vk_info.MinImageCount = guiInitInfo->MinImageCount;
        vk_info.ImageCount = guiInitInfo->ImageCount;
        vk_info.PipelineCache = guiInitInfo->PipelineCache; // Optional

        ImGui_ImplVulkan_Init(&vk_info);
    }
}

#endif // _WIN32