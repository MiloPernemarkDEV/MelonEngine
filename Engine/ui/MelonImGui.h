#ifndef MELONENGINE_MELON_IMGUI_H
#define MELONENGINE_MELON_IMGUI_H

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
        window(const char* title,  bool* p_open, ImGuiWindowFlags flags = ImGuiWindowFlags_None);
        ~window();
        [[nodiscard]] bool is_open() const;

        explicit operator bool() const { return open; }
    private:
        bool open = false;
    };
}

class Device;

class MelonImGui {
public:
    MelonImGui(Device* device, GLFWwindow* window, Pipelines* pipelines);
    void Init() const;
    void immediate_submit(const std::function<void(VkCommandBuffer cmd)> &func) const;
    void Draw(VkCommandBuffer cmd, VkImageView targetImageView) const;
    void draw_debug_ui(const char* title) const;

    static void draw_top_menu() ;
    static void add_window(const char* id, std::function<void()> func);
    static bool& window_open(const char* id);

    Device* _device;
    MelonImguiObjects* _uiSyncObjects;
    GLFWwindow* _window;
    Pipelines* _pipelines;
private:
    void set_window_state(const std::string&, bool value) const;
    std::vector<ComputeEffect*> _computeEffects;
    inline static std::unordered_map<std::string, std::function<void()>> editor_window_uis;
    float alpha = 1.0f;

    static void open_window(const char* id) ;

    inline static std::unordered_map<std::string, bool> window_states;
};

#endif //MELONENGINE_MELON_IMGUI_H
