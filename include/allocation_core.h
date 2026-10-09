#ifndef ALLOCATION_CORE_H
#define ALLOCATION_CORE_H

#include "chunk.h"

#include <cerrno>
#include <list>
#include <memory>
#include <new>

using AllocationList = std::list<allocation*>;

// Task 3 supplies the selected free list and its matching iterator.
// For Quick Fit, free_chunks is the selected partition's list.
// Pass free_chunks.end() when the strategy finds no suitable chunk.
inline void* allocate_chunk(std::size_t requested,
                            AllocationList& occupied,
                            AllocationList& free_chunks,
                            AllocationList::iterator candidate) {
    const std::size_t partition = round_to_partition(requested);
    if (partition == 0) {
        errno = EINVAL;
        return nullptr;
    }

    if (candidate != free_chunks.end()) {
        allocation* record = *candidate;
        if (record->size < partition) {
            errno = EINVAL;
            return nullptr;
        }
        // Moving a list node needs no allocation and preserves the metadata.
        occupied.splice(occupied.end(), free_chunks, candidate);
        record->used = requested;
        return record->space;
    }

    // Allocate all bookkeeping before sbrk: a metadata failure must not
    // leave an untracked region on the program break.
    std::unique_ptr<allocation> record;
    try {
        record.reset(new allocation{partition, requested, nullptr});
        occupied.push_back(record.get());
    } catch (const std::bad_alloc&) {
        errno = ENOMEM;
        return nullptr;
    }

    record->space = grow_heap(partition);
    if (record->space == nullptr) {
        occupied.pop_back();
        return nullptr;
    }

    void* space = record->space;
    record.release(); // Metadata is now owned by the allocator's lists.
    return space;
}

#endif
