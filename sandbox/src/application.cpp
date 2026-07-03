#include "Application.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

Application::Application(Arena* globalArena)
    : persistentArena(globalArena)
{
    platformWindow = persistentArena->add<PlatformWindow>();
}

bool Application::init() {
    if (!platformWindow->init()) { return false; }



    return true;
}

void Application::Run() {

    while (!platformWindow->window_should_close()) {
        platformWindow->poll_for_events();

        platformWindow->swap_buffers();
    }
}

void Application::terminate() {
    platformWindow->terminate();
}

