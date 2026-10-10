#include "memory.h"
#include "allocation_strategy.h"

#include <cassert>
#include <iostream>

extern AllocationList occupied_chunks;
extern AllocationList free_chunks;

// Temporary only for testing Task 4.
// Task 3 will provide the real version later.
FreeChunkSelection select_free_chunk(std::size_t) {
    return {free_chunks, free_chunks.end()};
}

int main() {
    char block1[32];
    char block2[64];

    allocation first = {32, 20, block1};
    allocation second = {64, 50, block2};

    occupied_chunks.push_back(&first);
    occupied_chunks.push_back(&second);

    dealloc(block1);

    assert(first.used == 0);

    assert(occupied_chunks.size() == 1);
    assert(occupied_chunks.front() == &second);

    assert(free_chunks.size() == 1);
    assert(free_chunks.front() == &first);

    std::cout << "firstfit dealloc test passed!" << std::endl;

    return 0;
}