//
// Created by milos on 2026-05-09.
//

#ifndef MELONENGINE_APPLICATION_H
#define MELONENGINE_APPLICATION_H

#include "WindowSystem.h"
#include "ArenaAlloc.h"

class Application {
public:
    Application(ArenaAlloc* globalArena);

    bool Init() const;
    void Run() const;
    void Terminate() const;
private:
    ArenaAlloc* globalArena;
    WindowSystem* windowSystem;
};


#endif //MELONENGINE_APPLICATION_H
