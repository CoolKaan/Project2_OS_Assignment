#include "memory.h"
#include "allocation_strategy.h"

#include <cstdlib>
#include <iostream>

// Task 3's search and Task 4's dealloc use these same lists in this file.
AllocationList occupied_chunks;
AllocationList free_chunks;

void* alloc(std::size_t chunk_size) {
    const std::size_t partition = round_to_partition(chunk_size);
    if (partition == 0) {
        errno = EINVAL;
        return nullptr;
    }
    FreeChunkSelection selected = select_free_chunk(partition);
    return allocate_chunk(chunk_size, occupied_chunks,
                          selected.list, selected.position);
}

// Task 3 - Allocation Strategy
// IMPLEMENT BELOW

// Task 4 - Deallocation
void dealloc(void* chunk) {
    for (auto it = occupied_chunks.begin();
         it != occupied_chunks.end();
         ++it) {

        allocation* record = *it;

        if (record->space == chunk) {
            // Mark the allocation as unused.
            record->used = 0;

            // Move the allocation from the occupied list
            // to the END of the free list.
            free_chunks.splice(
                free_chunks.end(),
                occupied_chunks,
                it
            );

            return;
        }
    }

    // The pointer was not found in the occupied list.
    std::cerr
        << "Fatal error: attempted to deallocate memory "
           "that was not allocated."
        << std::endl;

    std::exit(EXIT_FAILURE);
}