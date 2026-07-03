#ifndef MELON_APPLICATION_H
#define MELON_APPLICATION_H

#include "../../Engine/core/platform_window.h"
#include "../../Engine/core/arena_memory.h"

class Application {
public:
    explicit Application(Arena* globalArena);

    bool init();
    void Run();
    void terminate();
private:
    Arena* persistentArena = nullptr;
    PlatformWindow* platformWindow;
};


#endif // MELON_APPLICATION_H
