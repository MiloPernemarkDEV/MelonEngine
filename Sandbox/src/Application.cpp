#include "Application.h"
#include <chrono>
#include <thread>
#include "MelonImGui.h"

Application::Application(ArenaAlloc* globalArena)
    : persistentArena(globalArena)
{
    windowSystem = persistentArena->add<WindowSystem>();
}

bool Application::Init() {
    if (!windowSystem->Init()) { return false; }

    renderer = persistentArena->add<Renderer>(windowSystem->window);
    if (!renderer->Init()) { return false; }

    return true;
}

void Application::Run() const {

    while (!windowSystem->WindowShouldClose()) {

        windowSystem->PollForEvents();

        //todo if rendering stop put thread to sleep

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        renderer->DrawDebugUI();

        ImGui::Render();

        renderer->Draw();

        windowSystem->SwapBuffers();
    }
}

void Application::Terminate() const {
    windowSystem->Terminate();
    renderer->Terminate();
}
