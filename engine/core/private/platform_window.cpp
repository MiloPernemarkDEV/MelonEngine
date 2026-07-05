#include "pch.h"
#include "../public/app_icon.h"
#include "platform_window.h"
#include "logger.h"
#include <filesystem>

PlatformWindow::PlatformWindow()
    : window(nullptr), WINDOW_X(1400), WINDOW_Y(980), APP_NAME("MelonEngine | Vulkan 1.3.1")
{
}

bool PlatformWindow::init() {
    std::filesystem::current_path("C:/dev/melon_engine");
    glfwInit();
    SetupContext();
    window = glfwCreateWindow(WINDOW_X, WINDOW_Y, APP_NAME, nullptr, nullptr);
    ME_LOG(Info, "Created glfw window, width {0}, height {1}, name {2}", WINDOW_X, WINDOW_Y, APP_NAME);

    glfwMakeContextCurrent(window);

    if (!window) { return false; }
    return true;
}

bool PlatformWindow::window_should_close() const {
    return glfwWindowShouldClose(window);
}

void PlatformWindow::swap_buffers() const {
    glfwSwapBuffers(window);
}

void PlatformWindow::poll_for_events() const {
    glfwPollEvents();
}

void PlatformWindow::terminate() {
    glfwTerminate();
}

auto PlatformWindow::SetupContext() const -> void {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

auto PlatformWindow::createWindowIcon(const char* filename) const -> void {
    auto [data] = AppIcon::create_icon(filename);
    GLFWimage images[1];
    images[0].pixels = data.pixels;
    images[0].width = data.x;
    images[0].height = data.y;
    glfwSetWindowIcon(window, 1, images);
}
