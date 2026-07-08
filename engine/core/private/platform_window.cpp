#include "pch.h"
#include "../public/app_icon.h"
#include "platform_window.h"
#include "logger.h"
#include <filesystem>

bool Platform::init() {
#if !defined(WIN32)
    return glfw.init();
#else
    return win32.init();
#endif
}

bool Platform::window_should_close() const {
#if !defined(WIN32)
    return glfw.window_should_close();
#else
    return win32.window_should_close();
#endif
}

void Platform::terminate() {
#if !defined(WIN32)
    glfw.terminate();
#else
    win32.terminate();
#endif
}

void Platform::poll_for_events() {
#if !defined(WIN32)
    glfw.poll_for_events();
#else
    win32.poll_for_events();
#endif
}

void Platform::swap_buffers() const
{
#if !defined(WIN32)
    glfw.swap_buffers();
#else
    win32.swap_buffers();
#endif
}

void Platform::set_app_icon(const char* filename) {
#if defined(WIN32)
    glfw.set_app_icon(filename);
#else
    win32.set_app_icon(filename);
#endif
}

PlatformGlfw::PlatformGlfw()
    : window(nullptr), WINDOW_X(1400), WINDOW_Y(980), APP_NAME("MelonEngine | Vulkan 1.3.1")
{
}
bool PlatformGlfw::init() {
    std::filesystem::current_path("C:/dev/melon_engine");
    glfwInit();
    set_context();
    window = glfwCreateWindow(WINDOW_X, WINDOW_Y, APP_NAME, nullptr, nullptr);
    ME_LOG(Info, "Created glfw window, width {0}, height {1}, name {2}", WINDOW_X, WINDOW_Y, APP_NAME);

    glfwMakeContextCurrent(window);

    if (!window) { return false; }
    return true;
}

bool PlatformGlfw::window_should_close() const {
    return glfwWindowShouldClose(window);
}

void PlatformGlfw::swap_buffers() const {
    glfwSwapBuffers(window);
}

void PlatformGlfw::poll_for_events() const {
    glfwPollEvents();
}

void PlatformGlfw::terminate() {
    glfwTerminate();
}

auto PlatformGlfw::set_context() const -> void {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

auto PlatformGlfw::set_app_icon(const char* filename) const -> void {
    auto [data] = AppIcon::create_icon(filename);
    GLFWimage images[1];
    images[0].pixels = data.pixels;
    images[0].width = data.x;
    images[0].height = data.y;
    glfwSetWindowIcon(window, 1, images);
}

PlatformWin32::PlatformWin32()
{

}

bool PlatformWin32::init()
{
    return false;
}

bool PlatformWin32::window_should_close() const
{
    return false;
}

void PlatformWin32::swap_buffers() const
{

}

void PlatformWin32::poll_for_events() const
{

}

void PlatformWin32::terminate()
{

}

void PlatformWin32::set_app_icon(const char* filename)
{

}