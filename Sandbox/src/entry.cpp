#include "../../Engine/core/ArenaAlloc.h"
#include "Application.h"
#include <stdexcept>
#include <iostream>

int main() {
    ArenaAlloc globalMemory(400 * 1024 * 1024);
    auto* app = globalMemory.add<Application>(&globalMemory);

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