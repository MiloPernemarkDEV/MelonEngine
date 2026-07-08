#ifndef MELON_ARENA_MEMORY_H
#define MELON_ARENA_MEMORY_H

#include "defines.h"
#include <cstddef>
#include <memory>
#include <stdexcept>

class Arena {
public:
    explicit Arena(std::size_t size);
    ~Arena() = default;
    Arena(const Arena&) = default;

    void free();

    template<typename T, typename... Args>
    auto add(Args&&... args) -> T* {

        const std::size_t incomingSize = sizeof(T);

        auto* startPtr = static_cast<void*>(buffer + offset);
        auto* alignedPtr = startPtr;

        std::size_t space = totalSize - offset;

        if (std::align(alignof(T), incomingSize, alignedPtr, space)) {

            const std::size_t padding =
                reinterpret_cast<uintptr_t>(alignedPtr)
                - reinterpret_cast<uintptr_t>(startPtr);

            offset += padding + incomingSize;

            return new(alignedPtr) T(std::forward<Args>(args)...);
        }

        throw std::runtime_error("Arena Allocator is Full or alignment failed!");
    }
private:
    std::size_t totalSize;
    std::size_t offset;
    char* buffer;
};

inline Arena g_GlobalArena{ 400_MB };

#endif // MELON_ARENA_MEMORY_H
