#ifndef CHUNK_H
#define CHUNK_H

#include <cstddef>

// Fixed partition sizes in bytes. Every request is rounded up to one of these.
constexpr std::size_t PARTITION_SIZES[] = {32, 64, 128, 256, 512};
constexpr int NUM_PARTITIONS = 5;

// Accounting record for one chunk of memory.
// Each record is heap-allocated with new and referenced by pointer, so
// list operations like erase()/pop_back() never run a destructor that
// could release the underlying memory.
struct allocation {
    std::size_t size;   // total chunk size (the partition size)
    std::size_t used;   // bytes the caller actually requested (0 when free)
    void* space;        // start address of the chunk, obtained from sbrk()
};

// Rounds a request up to the smallest partition that fits it.
// Returns 0 if the request exceeds the largest partition.
std::size_t round_to_partition(std::size_t request);

// Grows the heap by `size` bytes using sbrk(). Returns nullptr on failure.
void* grow_heap(std::size_t size);

#endif