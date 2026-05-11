//
// Created by milos on 2026-05-09.
//

#ifndef MELONENGINE_APPLICATION_H
#define MELONENGINE_APPLICATION_H

#include "WindowSystem.h"
#include "Renderer.h"
#include "ArenaAlloc.h"

class Application {
public:
    explicit Application(ArenaAlloc* globalArena);

    bool Init();
    void Run() const;
    void Terminate() const;
private:
    Renderer* renderer = nullptr;
    ArenaAlloc* persistentArena;
    WindowSystem* windowSystem;
};


#endif //MELONENGINE_APPLICATION_H
