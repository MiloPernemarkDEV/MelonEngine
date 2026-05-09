#include "Application.h"

Application::Application(ArenaAlloc* globalArena)
    : globalArena(globalArena)
{
    windowSystem = globalArena->add<WindowSystem>();
}

bool Application::Init() const {
    if (!windowSystem->Init()) { return false; }

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
}
