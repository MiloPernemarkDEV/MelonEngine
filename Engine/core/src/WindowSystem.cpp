#include "pch.h"
#include "WindowSystem.h"
#include "ImageUtil.h"

WindowSystem::WindowSystem()
    : window(nullptr), WINDOW_X(1400), WINDOW_Y(980), APP_NAME("MelonLife | OpenGL")
{
}

// ReSharper disable once CppMemberFunctionMayBeConst
// @Todo
// refactor the current path from my own system path to an agnostic one
bool WindowSystem::Init() {
    std::filesystem::current_path("C:/msys64/home/milos/dev/C++/MelonEngine");
    glfwInit();
    SetupContext();
    window = glfwCreateWindow(WINDOW_X, WINDOW_Y, APP_NAME, nullptr, nullptr);
    createWindowIcon("Assets/Icons/melonengine.png");

    glfwMakeContextCurrent(window);

    if (!window) { return false; }
    return true;
}

bool WindowSystem::WindowShouldClose() const {
    return glfwWindowShouldClose(window);
}

void WindowSystem::SwapBuffers() const {
    glfwSwapBuffers(window);
}

void WindowSystem::PollForEvents() const {
    glfwPollEvents();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void WindowSystem::Terminate() {
    glfwTerminate();
}

auto WindowSystem::SetupContext() const -> void {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

auto WindowSystem::createWindowIcon(const char* filename) const -> void {
    auto [data] = ImageUtil::CreateIcon(filename);
    GLFWimage images[1];
    images[0].pixels = data.pixels;
    images[0].width = data.x;
    images[0].height = data.y;
    glfwSetWindowIcon(window, 1, images);
}
