#ifndef MELONENGINE_APPLICATION_H
#define MELONENGINE_APPLICATION_H

#include "../../Engine/core/WindowSystem.h"
#include "../../Engine/graphics/Renderer.h"
#include "../../Engine/core/ArenaAlloc.h"

class Application {
public:
    explicit Application(ArenaAlloc* globalArena);

    bool init();
    void Run();
    void terminate();
private:
    bool show_tools = true;
    Renderer* renderer = nullptr;
    ArenaAlloc* persistentArena;
    WindowSystem* windowSystem;
};


#endif //MELONENGINE_APPLICATION_H
