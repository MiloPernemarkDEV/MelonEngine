#include "application.h"

Application::Application(Arena* globalArena)
    : persistentArena(globalArena)
{
    platformWindow = persistentArena->add<PlatformWindow>();
}

bool Application::init() {
    if (!platformWindow->init()) { return false; }

    renderer = persistentArena->add<Renderer>(platformWindow->WINDOW_X, platformWindow->WINDOW_Y);
    renderer->init(platformWindow->window);


    return true;
}

void Application::Run() {

    while (!platformWindow->window_should_close()) {
        platformWindow->poll_for_events();

        renderer->draw();

        platformWindow->swap_buffers();
    }
}

void Application::terminate() {
    renderer->terminate();
    platformWindow->terminate();
}

