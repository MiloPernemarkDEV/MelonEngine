#include "pch.h"
#include "arena_memory.h"

Arena::Arena(const std::size_t size)
    : totalSize(size), offset(0)
{
    buffer = static_cast<char*>(::operator new(totalSize));
}

void Arena::free() {
    if (buffer) {
        ::operator delete(buffer);
        totalSize = 0;
        offset = 0;
        buffer = nullptr;
    }
}
