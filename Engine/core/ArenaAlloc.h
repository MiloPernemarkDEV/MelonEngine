//
// Created by milos on 2026-05-08.
//

#ifndef MELONENGINE_ARENAALLOC_H
#define MELONENGINE_ARENAALLOC_H

#include <cstddef>
#include <memory>
#include <stdexcept>

class ArenaAlloc {
public:
    explicit ArenaAlloc(std::size_t size);
    ~ArenaAlloc() = default;
    ArenaAlloc(const ArenaAlloc&) = default;

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


#endif //MELONENGINE_ARENAALLOC_H
