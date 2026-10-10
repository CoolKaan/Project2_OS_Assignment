# Project2_OS_Assignment

## Task 2 - Memory Allocation

Implemented the `alloc(std::size_t)` entry point in each strategy source,
shared allocation bookkeeping in `include/allocation_core.h`, and partition
rounding / Linux `sbrk()` handling in `src/chunk.cpp`.

Requests from 1 through 512 bytes round up to 32, 64, 128, 256 or 512 bytes.
Zero and oversized requests return `nullptr` with `errno = EINVAL`.
An existing free chunk keeps its full partition size: it is not split.
Its record moves to the occupied list and `used` becomes the requested size.
If no free chunk fits, bookkeeping is prepared before requesting exactly one
partition from `sbrk()`. Allocation failure returns `nullptr`; a failed heap
request removes the provisional record. Memory is not returned to the OS.

### Integration with the remaining tasks

Task 3 must implement `select_free_chunk(partition)` separately in each
strategy source, using the lists already declared there. The interface is
in `include/allocation_strategy.h`. It must search before heap growth,
return a matching iterator without erasing it, or return a list's `end()`
when no block fits. Quick Fit returns the relevant size-class list.

### Build and check Task 2

On Linux, WSL, or an RMIT teaching server with GNU Make and g++:

```sh
make test-task2
```

## Task 4 - Memory Deallocation

Task 4 implements the `dealloc(void*)` function for each memory allocation
strategy.

The function searches `occupied_chunks` for the allocation record whose
`space` pointer matches the pointer passed to `dealloc()`.

When the matching allocation is found, its `used` field is set to `0` to mark
the chunk as no longer in use. The allocation record is then removed from the
occupied list and transferred to the end of the appropriate free list.

The memory itself is not returned to the operating system. Instead, the freed
chunk remains available so that it can be reused by future calls to `alloc()`.

For First Fit and Best Fit, deallocated chunks are added to the end of the
single `free_chunks` list.

For Quick Fit, the allocation record is returned to the free list associated
with its partition size:

- 32 bytes
- 64 bytes
- 128 bytes
- 256 bytes
- 512 bytes

If the pointer passed to `dealloc()` cannot be found in `occupied_chunks`,
the program terminates because attempting to free memory that is not currently
allocated is treated as a fatal error.

## Task 4 Testing

Task 4 can be tested independently from the complete program using the
deallocation tests in the `tests` directory.

The tests create allocation records manually, place them into
`occupied_chunks`, call `dealloc()`, and verify that:

- the matching allocation is removed from `occupied_chunks`
- the allocation's `used` value becomes `0`
- the allocation is moved to the correct free list
- remaining allocated chunks are not affected
- Quick Fit returns the chunk to the correct partition list

The tests allow the deallocation functionality to be checked separately while
other tasks are still being developed.

Run all Task 4 tests using:

```sh
make test-task4
```


The independent test uses `-std=c++11 -Wall -Werror` and checks partition
boundaries, writable new chunks, reuse of a larger free partition, metadata,
and invalid requests. A second test simulates `sbrk()` failure and checks
that no allocation record is left behind. Tests exercise the component without requiring
the unfinished strategy searches, parser, or main program.

Once the other tasks are integrated:

```sh
make all
./firstfit datafile
./bestfit datafile
./quickfit datafile
```
