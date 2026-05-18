#ifndef MELONENGINE_RENDERER_H
#define MELONENGINE_RENDERER_H

#include "Device.h"
#include <GLFW/glfw3.h>
#include "Descriptors.h"
#include "Pipelines.h"
#include "MelonImGui.h"
#include "RenderTypes.h"
#include <vector>

class Renderer {
public:
    explicit Renderer(GLFWwindow* window);
    void draw_frame();
    bool init();
    void terminate();
    void draw_editor_windows();
    void draw_debug_ui();
    void draw_menu_ui();
    void draw_tilemap_ui();
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
