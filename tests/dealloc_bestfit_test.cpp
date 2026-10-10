#include "memory.h"
#include "allocation_strategy.h"

#include <cassert>
#include <iostream>

extern AllocationList occupied_chunks;
extern AllocationList free_chunks;

int main() {
    char block1[32];
    char block2[64];

    allocation first = {32, 20, block1};
    allocation second = {64, 50, block2};

    // Pretend both chunks are currently allocated.
    occupied_chunks.push_back(&first);
    occupied_chunks.push_back(&second);

    // Deallocate the first chunk.
    dealloc(block1);

    // Check that it is marked unused.
    assert(first.used == 0);

    // Only the second chunk should remain occupied.
    assert(occupied_chunks.size() == 1);
    assert(occupied_chunks.front() == &second);

    // First chunk should now be in the free list.
    assert(free_chunks.size() == 1);
    assert(free_chunks.front() == &first);

    std::cout << "dealloc test passed!" << std::endl;

    return 0;
}