#ifndef MELONENGINE_MELONIMGUI_H
#define MELONENGINE_MELONIMGUI_H

#include <functional>
#include "RenderTypes.h"
#include <GLFW/glfw3.h>
#include "Pipelines.h"

namespace ui
{
    bool begin_window(const char* title);
    void end_window();

    class window {
    public:
        window(const char* title);
        ~window();
        bool is_open() const;
        operator bool() const { return open; }
    private:
        bool open = false;
    };
}

class Device;

class MelonImGui {
public:
    MelonImGui(Device* device, GLFWwindow* window, Pipelines* pipelines);
    void Init();
    void immediate_submit(std::function<void(VkCommandBuffer cmd)> func);
    void Draw(VkCommandBuffer cmd, VkImageView targetImageView);
    void draw_debug_ui();
    void init_imgui_styles();
    void draw_app_top_menu();

    Device* _device;
    MelonImguiObjects* _uiSyncObjects;
    GLFWwindow* _window;
    Pipelines* _pipelines;
private:
    std::vector<ComputeEffect*> _computeEffects;
    float _alpha = 1.0f;
    bool bStyleDark_ = false;
};

#endif //MELONENGINE_MELONIMGUI_H
