#include "memory.h"
#include "allocation_strategy.h"

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
