#include "application.h"

Application::Application(Arena* globalArena)
    : persistentArena(globalArena)
{
    platform = persistentArena->add<Platform>();
    renderer = persistentArena->add<Renderer>(platform->WIDTH(), platform->HEIGHT());
}

bool Application::init() {
    if (!platform->init() ) {
        return false;
    }

    if (!renderer->init(platform->HANDLE()) ) {
        return false;
    }

    return true;
}

void Application::Run() {
    while (!Platform::window_should_close()) {
        TimeUtil::update();
        Platform::poll_for_events();

        renderer->draw();
    }
}

void Application::terminate() {
    renderer->terminate();
    platform->terminate();
}

