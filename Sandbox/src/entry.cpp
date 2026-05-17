#include "ArenaAlloc.h"
#include "Application.h"
#include <stdexcept>
#include <iostream>

int main() {
    ArenaAlloc globalMemory(400 * 1024 * 1024);
    auto* app = globalMemory.add<Application>(&globalMemory);

    if (!app->Init()) {
        return EXIT_FAILURE;
    }

    try {
        app->Run();
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    app->Terminate();
    globalMemory.Release();
    return EXIT_SUCCESS;
}