#include "ArenaAlloc.h"
#include "Application.h"
#include <stdexcept>

int main() {
    ArenaAlloc globalMemory(400 * 1024 * 1024);
    auto* app = globalMemory.add<Application>(&globalMemory);
    if (!app->Init()) {
        return EXIT_FAILURE;
    }

    app->Run();
    app->Terminate();

    globalMemory.Release();
    return EXIT_SUCCESS;
}