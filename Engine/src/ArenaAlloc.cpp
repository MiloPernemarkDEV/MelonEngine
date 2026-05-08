#include "ArenaAlloc.h"
#include "Debug.h"

ArenaAlloc::ArenaAlloc(const std::size_t size)
    : totalSize(size), offset(size)
{
    try {
        buffer = static_cast<char*>(::operator new(totalSize));
    }
    catch (std::bad_alloc&) {
        Debug::Log(LogLevel::WARNING,"Failed to allocate memory to the Arena!");
    }
}

ArenaAlloc::~ArenaAlloc() {
    if (buffer) {
        ::operator delete(buffer);
        totalSize = 0;
        offset = 0;
        buffer = nullptr;
    }
    totalSize = 0;
    offset = 0;
    buffer = nullptr;
}