#include "memory.h"
#include "allocation_strategy.h"

#include <cstdlib>
#include <iostream>

// Task 3's search and Task 4's dealloc use these same lists in this file.
namespace {
AllocationList occupied_chunks;
AllocationList free_chunks;
AllocationCleanup cleanup{occupied_chunks, &free_chunks, 1};
}

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
FreeChunkSelection select_free_chunk(std::size_t partition) {
    auto best = free_chunks.end();

    for (auto it = free_chunks.begin(); it != free_chunks.end(); ++it) {
        if ((*it)->size >= partition &&
            (best == free_chunks.end() || (*it)->size < (*best)->size)) {
            best = it;

            if ((*it)->size == partition) {
                break;
            }
        }
    }
    return {free_chunks, best};
}


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

void print_lists() {
    std::cout << "Allocated list:" << std::endl;

    for (allocation* record : occupied_chunks) {
        std::cout
            << "Address: " << record->space
            << ", Size: " << record->size
            << ", Used: " << record->used
            << std::endl;
    }

    std::cout << "Free list:" << std::endl;

    for (allocation* record : free_chunks) {
        std::cout
            << "Address: " << record->space
            << ", Size: " << record->size
            << ", Used: " << record->used
            << std::endl;
    }
}
