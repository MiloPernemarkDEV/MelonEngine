#include "ArenaAlloc.h"
#include "Application.h"
#include <filesystem>\\

int main() {
    std::filesystem::current_path("C:/msys64/home/milos/dev/C++/MelonEngine");
    ArenaAlloc globalMemory(400 * 1024 * 1024);
    auto* app = globalMemory.add<Application>(&globalMemory);
    if (!app->Init()) {
        return -1;
    }

    app->Run();
    app->Terminate();

    globalMemory.Release();
    return 0;
}