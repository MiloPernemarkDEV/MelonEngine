#include "Application.h"
#include <chrono>
#include <thread>
#include "../../Engine/ui/MelonImGui.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

Application::Application(ArenaAlloc* globalArena)
    : persistentArena(globalArena)
{
    windowSystem = persistentArena->add<WindowSystem>();
}

bool Application::init() {
    if (!windowSystem->init()) { return false; }

    renderer = persistentArena->add<Renderer>(windowSystem->window);
    if (!renderer->init()) { return false; }

    return true;
}

void Application::Run() {

    while (!windowSystem->window_should_close()) {

        windowSystem->poll_for_events();

        //todo if rendering stop put thread to sleep

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        renderer->draw_editor_windows();

        ImGui::Render();

        renderer->draw_frame();
        windowSystem->swap_buffers();
    }
}

void Application::terminate() {
    windowSystem->terminate();
    renderer->terminate();
}

