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
    PlatformGlfw* platformWindow;
    Renderer* renderer;
};


#endif // MELON_APPLICATION_H
