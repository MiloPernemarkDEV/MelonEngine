#ifndef MELONENGINE_RENDERER_H
#define MELONENGINE_RENDERER_H

#include "Device.h"
#include <GLFW/glfw3.h>

class Renderer {
public:
    explicit Renderer(GLFWwindow* window);
    void Draw();
    bool Init();
    void Terminate();
private:
    void DrawBackground(VkCommandBuffer cmd);
    GLFWwindow* _window;
    Device _device = nullptr;
};

#endif //MELONENGINE_RENDERER_H
