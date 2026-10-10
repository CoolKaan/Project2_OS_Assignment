#include "memory.h"
#include "allocation_strategy.h"

#include <cstdlib>
#include <iostream>


// Task 3's search and Task 4's dealloc use these same lists in this file.
AllocationList occupied_chunks;
AllocationList free_chunks[NUM_PARTITIONS];

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
    // Search for the chunk in the allocated/occupied list.
    for (auto it = occupied_chunks.begin();
         it != occupied_chunks.end();
         ++it) {

        allocation* record = *it;

        // Check whether this is the chunk the caller wants to free.
        if (record->space == chunk) {

            // Find which Quick Fit free list this chunk belongs to.
            int partition_index = -1;

            for (int i = 0; i < NUM_PARTITIONS; ++i) {
                if (record->size == PARTITION_SIZES[i]) {
                    partition_index = i;
                    break;
                }
            }

            // This should never happen if allocation was performed correctly.
            if (partition_index == -1) {
                std::cerr
                    << "Fatal error: invalid partition size."
                    << std::endl;

                std::exit(EXIT_FAILURE);
            }

            // Mark the chunk as unused.
            record->used = 0;

            // Move it from the occupied list to the END of
            // the appropriate Quick Fit free list.
            free_chunks[partition_index].splice(
                free_chunks[partition_index].end(),
                occupied_chunks,
                it
            );

            return;
        }
    }

    // Pointer was not found in the occupied list.
    std::cerr
        << "Fatal error: attempted to deallocate memory "
           "that was not allocated."
        << std::endl;

    std::exit(EXIT_FAILURE);
}