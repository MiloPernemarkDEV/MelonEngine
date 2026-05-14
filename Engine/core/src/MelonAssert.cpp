#include "pch.h"
#include "MelonAssert.h"


namespace Melon
{
    void HandleMelonAssert(const char* expr,
        const char* msg,
        const char* file,
        const int line)
    {
        std::cerr << "========================================\n";
        std::cerr << "MELON ASSERTION FAILED\n";
        std::cerr << "Expression: " << expr << "\n";

        if (msg) {
            std::cerr << "Message: " << msg << "\n";
        }

        std::cerr << "File: " << file << "\n";
        std::cerr << "Line: " << line << "\n";
        std::cerr << "========================================\n";

#if defined(_MSC_VER)
        __debugbreak();
#else
        std::abort();
#endif
    }
}
