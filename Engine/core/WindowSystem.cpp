#include "pch.h"
#include "WindowSystem.h"
#include "AppIcon.h"

WindowSystem::WindowSystem()
    : window(nullptr), WINDOW_X(1400), WINDOW_Y(980), APP_NAME("MelonEngine | Vulkan 1.3.1")
{
}

// ReSharper disable once CppMemberFunctionMayBeConst
// @Todo refactor path from hardcoding my own path
bool WindowSystem::init() {
    std::filesystem::current_path("C:/msys64/home/milos/dev/C++/MelonEngine");
    glfwInit();
    SetupContext();
    window = glfwCreateWindow(WINDOW_X, WINDOW_Y, APP_NAME, nullptr, nullptr);
    createWindowIcon("Assets/Icons/melonengine.png");

    glfwMakeContextCurrent(window);

    if (!window) { return false; }
    return true;
}

bool WindowSystem::window_should_close() const {
    return glfwWindowShouldClose(window);
}

void WindowSystem::swap_buffers() const {
    glfwSwapBuffers(window);
}

void WindowSystem::poll_for_events() const {
    glfwPollEvents();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void WindowSystem::terminate() {
    glfwTerminate();
}

auto WindowSystem::SetupContext() const -> void {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

auto WindowSystem::createWindowIcon(const char* filename) const -> void {
    auto [data] = AppIcon::CreateIcon(filename);
    GLFWimage images[1];
    images[0].pixels = data.pixels;
    images[0].width = data.x;
    images[0].height = data.y;
    glfwSetWindowIcon(window, 1, images);
}
