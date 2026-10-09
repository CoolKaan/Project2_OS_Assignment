#include "allocation_core.h"

#include <cassert>
#include <cstdint>

// GNU ld redirects only this test executable's sbrk calls here.
extern "C" void* __wrap_sbrk(std::intptr_t increment) {
    assert(increment == 64);
    errno = ENOMEM;
    return reinterpret_cast<void*>(-1);
}

int main() {
    AllocationList occupied;
    AllocationList available;
    assert(allocate_chunk(33, occupied, available, available.end()) == nullptr);
    assert(errno == ENOMEM);
    assert(occupied.empty());
    assert(available.empty());
}
