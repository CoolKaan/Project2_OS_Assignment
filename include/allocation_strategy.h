#ifndef ALLOCATION_STRATEGY_H
#define ALLOCATION_STRATEGY_H

#include "allocation_core.h"

struct FreeChunkSelection {
    AllocationList& list;
    AllocationList::iterator position;
};

// Implement in each strategy file for Task 3. Search without removing the
// candidate. Return {list, list.end()} when no chunk fits. For Quick Fit,
// return the appropriate size-class list (including when that list is empty).
FreeChunkSelection select_free_chunk(std::size_t partition);

#endif
