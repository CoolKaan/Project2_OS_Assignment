#include "allocation_core.h"

#include <cassert>
#include <cstring>
#include <limits>

int main() {
    assert(round_to_partition(0) == 0);
    assert(round_to_partition(513) == 0);
    assert(round_to_partition(std::numeric_limits<std::size_t>::max()) == 0);
    std::size_t previous = 0;
    for (std::size_t size : PARTITION_SIZES) {
        assert(round_to_partition(previous + 1) == size);
        assert(round_to_partition(size) == size);
        previous = size;
    }
    assert(grow_heap(33) == nullptr);

    AllocationList occupied;
    AllocationList available;
    for (std::size_t size : PARTITION_SIZES) {
        void* pointer = allocate_chunk(size - 1, occupied, available,
                                       available.end());
        assert(pointer != nullptr);
        assert(occupied.back()->space == pointer);
        assert(occupied.back()->size == size);
        assert(occupied.back()->used == size - 1);
        std::memset(pointer, 0xA5, size);
    }
    assert(occupied.size() == 5);

    // Simulate Task 4 returning a non-last chunk to a free list.
    auto returned = occupied.begin();
    ++returned; // 64-byte partition
    allocation* record = *returned;
    record->used = 0;
    available.splice(available.end(), occupied, returned);
    void* reused = allocate_chunk(20, occupied, available, available.begin());
    assert(reused == record->space);
    assert(occupied.back() == record);
    assert(record->size == 64 && record->used == 20);
    assert(available.empty());

    assert(allocate_chunk(0, occupied, available, available.end()) == nullptr);
    assert(allocate_chunk(513, occupied, available, available.end()) == nullptr);
    assert(occupied.size() == 5);
    for (allocation* item : occupied) {
        delete item; // sbrk memory is never passed to delete/free.
    }
}
