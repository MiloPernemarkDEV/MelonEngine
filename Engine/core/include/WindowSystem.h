#ifndef MELONENGINE_WINDOWSYSTEM_H
#define MELONENGINE_WINDOWSYSTEM_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class WindowSystem {
public:
    WindowSystem();
    ~WindowSystem() = default;

    WindowSystem(WindowSystem&) = delete;
    WindowSystem& operator=(WindowSystem&) = delete;

    WindowSystem(WindowSystem&&) noexcept = default;

    bool Init();
    bool WindowShouldClose() const;
    void SwapBuffers() const;
    void PollForEvents() const;
    void Terminate();
private:
    auto setupGLContext() const -> void;
    auto createWindowIcon(const char* filename) const -> void;
    GLFWwindow* window;
    const int WINDOW_X;
    const int WINDOW_Y;
    const char* APP_NAME;
};

#endif //MELONENGINE_WINDOWSYSTEM_H
