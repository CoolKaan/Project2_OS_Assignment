#include "memory.h"
#include "allocation_strategy.h"

#include <cassert>
#include <iostream>

extern AllocationList occupied_chunks;
extern AllocationList free_chunks[NUM_PARTITIONS];

// Temporary while Task 3 is not implemented.
FreeChunkSelection select_free_chunk(std::size_t partition) {
    for (int i = 0; i < NUM_PARTITIONS; ++i) {
        if (PARTITION_SIZES[i] == partition) {
            return {free_chunks[i], free_chunks[i].end()};
        }
    }

    return {free_chunks[0], free_chunks[0].end()};
}

int main() {
    char block1[128];
    char block2[64];

    allocation first = {128, 100, block1};
    allocation second = {64, 50, block2};

    // Pretend both chunks are currently allocated.
    occupied_chunks.push_back(&first);
    occupied_chunks.push_back(&second);

    // Deallocate the 128-byte chunk.
    dealloc(block1);

    // Chunk should now be unused.
    assert(first.used == 0);

    // Second chunk should still be occupied.
    assert(occupied_chunks.size() == 1);
    assert(occupied_chunks.front() == &second);

    // Find the 128-byte Quick Fit partition.
    int index = -1;

    for (int i = 0; i < NUM_PARTITIONS; ++i) {
        if (PARTITION_SIZES[i] == 128) {
            index = i;
            break;
        }
    }

    assert(index != -1);

    // Freed chunk should be in the correct Quick Fit free list.
    assert(free_chunks[index].size() == 1);
    assert(free_chunks[index].front() == &first);

    std::cout << "quickfit dealloc test passed!" << std::endl;

    return 0;
}