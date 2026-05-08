#ifndef MELONENGINE_WINDOWSYSTEM_H
#define MELONENGINE_WINDOWSYSTEM_H

#include <GLFW/glfw3.h>

class WindowSystem {
public:
    WindowSystem();
    ~WindowSystem() = default;

    WindowSystem(WindowSystem&) = delete;
    WindowSystem& operator=(WindowSystem&) = delete;

    WindowSystem(WindowSystem&&) noexcept = default;

    void Init();
    void Terminate();
private:
    GLFWwindow* window;
    const int WINDOW_X;
    const int WINDOW_Y;
    const char* APP_NAME;
};

#endif //MELONENGINE_WINDOWSYSTEM_H
