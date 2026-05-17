#ifndef MELONENGINE_RENDERER_H
#define MELONENGINE_RENDERER_H

#include "Device.h"
#include <GLFW/glfw3.h>
#include "Descriptors.h"
#include "Pipelines.h"
#include "MelonImGui.h"
#include "VulkanTypes.h"
#include <vector>

class Renderer {
public:
    explicit Renderer(GLFWwindow* window);
    void Draw();
    bool Init();
    void Terminate();
    void DrawDebugUI();
private:
    void DrawBackground(VkCommandBuffer cmd);
    void DrawGeometry(VkCommandBuffer cmd);
    GLFWwindow* _window;
    Device _device;
    Descriptors _descriptors;
    Pipelines _pipelines;
    MelonImGui _melonImgui;
};

#endif //MELONENGINE_RENDERER_H
