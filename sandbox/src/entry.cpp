#include "engine.h"
#include "file_util.h"
#include "application.h"
#include <stdexcept>
#include <iostream>

int main() {
    File::FileOp file;
    if (file.open("NewFile.txt")) {
        file.write_str("THIS IS SOME STRING TO CHECK IF THE FILE SIZE FUNCTIONS WORK!");

        constexpr std::size_t maxSize{0_MB};
        if (file.size() > maxSize) {
            ME_LOG(Warning, "File size is too big, {} bytes", file.size());
        }
    }

    Arena globalMemory(400_MB);
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