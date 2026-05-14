#ifndef MELONENGINE_MELONIMGUI_H
#define MELONENGINE_MELONIMGUI_H

#include <vulkan/vulkan.h>
#include <functional>
#include "VulkanTypes.h"
#include <GLFW/glfw3.h>

class Device;

class MelonImgui {
public:
    MelonImgui(Device* device, GLFWwindow* window);
    void Init();
    void ImmediateSubmit(std::function<void(VkCommandBuffer cmd)> func);

    Device* _device;
    MelonImguiObjects* _uiSyncObjects;
    GLFWwindow* _window;
};

#endif //MELONENGINE_MELONIMGUI_H
