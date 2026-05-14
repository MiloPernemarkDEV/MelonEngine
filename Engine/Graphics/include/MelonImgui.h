#ifndef MELONENGINE_MELONIMGUI_H
#define MELONENGINE_MELONIMGUI_H

#include <vulkan/vulkan.h>
#include <functional>
#include "VulkanTypes.h"
#include <GLFW/glfw3.h>
#include "Pipelines.h"

class Device;

class MelonImGui {
public:
    MelonImGui(Device* device, GLFWwindow* window, Pipelines* pipelines);
    void Init();
    void ImmediateSubmit(std::function<void(VkCommandBuffer cmd)> func);
    void Draw(VkCommandBuffer cmd, VkImageView targetImageView);
    void DrawDebugUI();
    void SetImGuiStyleConfig();

    Device* _device;
    MelonImguiObjects* _uiSyncObjects;
    GLFWwindow* _window;
    Pipelines* _pipelines;
private:
    std::vector<ComputeEffect*> _computeEffects;
    float _alpha = 1.0f;
    bool bStyleDark_ = true;
};

#endif //MELONENGINE_MELONIMGUI_H
