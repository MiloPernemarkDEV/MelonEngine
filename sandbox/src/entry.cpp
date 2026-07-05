#include "../../Engine/core/arena_memory.h"
#include "application.h"
#include <stdexcept>
#include <iostream>

int main() {
    Arena globalMemory(400 * 1024 * 1024);
    Arena* arenaPtr = &globalMemory;
    auto* app = globalMemory.add<Application>(arenaPtr);

    if (!app->init()) {
        return EXIT_FAILURE;
    }

    try {
        app->Run();
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    app->terminate();
    globalMemory.free();
    return EXIT_SUCCESS;
}