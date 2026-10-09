#include "chunk.h"

#include <cerrno>
#include <cstdint>
#include <unistd.h>

std::size_t round_to_partition(std::size_t request) {
    if (request == 0) {
        return 0;
    }
    for (std::size_t size : PARTITION_SIZES) {
        if (request <= size) {
            return size;
        }
    }
    return 0;
}

void* grow_heap(std::size_t size) {
    // Only complete fixed partitions may be requested from the OS.
    bool valid = false;
    for (std::size_t partition : PARTITION_SIZES) {
        valid = valid || size == partition;
    }
    if (!valid) {
        errno = EINVAL;
        return nullptr;
    }

    void* space = ::sbrk(static_cast<std::intptr_t>(size));
    if (space == reinterpret_cast<void*>(-1)) {
        return nullptr;
    }
    return space;
}
