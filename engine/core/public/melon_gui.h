#ifndef MELON_MELON_GUI_H
#define MELON_MELON_GUI_H

struct GuiInitInfo {
    uint32_t                        ApiVersion;
    VkInstance                      Instance;
    VkPhysicalDevice                PhysicalDevice;
    VkDevice                        Device;
    uint32_t                        QueueFamily;
    VkQueue                         Queue;
    VkDescriptorPool                DescriptorPool;
    uint32_t                        DescriptorPoolSize; // Might be handled internally by imgui
    uint32_t                        MinImageCount;
    uint32_t                        ImageCount;
    VkPipelineCache                 PipelineCache; // Optional
};

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace MelonGUI {
    void Init(HWND windowHandle, const GuiInitInfo* guiInitInfo);
}

#endif //MELON_MELON_GUI_H
