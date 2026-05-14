#ifndef MELONENGINE_RENDERER_H
#define MELONENGINE_RENDERER_H

#include "Device.h"
#include <GLFW/glfw3.h>
#include "Descriptors.h"
#include "Pipelines.h"
#include "MelonImgui.h"

class Renderer {
public:
    explicit Renderer(GLFWwindow* window);
    void Draw();
    bool Init();
    void Terminate();
private:
    void DrawBackground(VkCommandBuffer cmd);
    GLFWwindow* _window;
    Device _device;
    Descriptors _descriptors;
    Pipelines _pipelines;
    MelonImgui _melonImgui;
};

#endif //MELONENGINE_RENDERER_H
