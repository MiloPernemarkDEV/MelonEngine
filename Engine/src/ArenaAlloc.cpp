#include "ArenaAlloc.h"
#include "Debug.h"

ArenaAlloc::ArenaAlloc(const std::size_t size)
    : totalSize(size), offset(0)
{
    buffer = static_cast<char*>(::operator new(totalSize));
}

void ArenaAlloc::Release() {
    if (buffer) {
        ::operator delete(buffer);
        totalSize = 0;
        offset = 0;
        buffer = nullptr;
    }
}
