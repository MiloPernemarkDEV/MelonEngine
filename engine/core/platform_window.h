#ifndef MELON_PLATFORM_WINDOW_H
#define MELON_PLATFORM_WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class PlatformWindow {
public:
    PlatformWindow();
    ~PlatformWindow() = default;

    PlatformWindow(PlatformWindow&) = delete;
    PlatformWindow& operator=(PlatformWindow&) = delete;

    PlatformWindow(PlatformWindow&&) noexcept = default;

    bool init();
    bool window_should_close() const;
    void swap_buffers() const;
    void poll_for_events() const;
    void terminate();
    GLFWwindow* window;

    const int WINDOW_X;
    const int WINDOW_Y;
   
private:
    auto SetupContext() const -> void;
    auto createWindowIcon(const char* filename) const -> void;
    const char* APP_NAME;
};

#endif // MELON_PLATFORM_WINDOW_H
