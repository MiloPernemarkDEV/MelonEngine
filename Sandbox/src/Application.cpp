#include "Application.h"

Application::Application(ArenaAlloc* globalArena)
    : persistentArena(globalArena)
{
    windowSystem = globalArena->add<WindowSystem>();
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


        windowSystem->SwapBuffers();
    }
}

void Application::Terminate() const {
    windowSystem->Terminate();
    renderer->Terminate();
}
