#ifndef MELON_APPLICATION_H
#define MELON_APPLICATION_H

#include "engine.h"

class Application {
public:
    explicit Application(Arena* globalArena);

    bool init();
    void Run();
    void terminate();
private:
    Arena* persistentArena = nullptr;
    Renderer* renderer = nullptr;
    Platform* platform = nullptr;

    void showcase_features();
};


#endif // MELON_APPLICATION_H
