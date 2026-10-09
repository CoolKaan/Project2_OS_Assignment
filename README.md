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

Task 4 must implement `dealloc()` in those same files, searching
`occupied_chunks` by pointer and transferring the record to the end of the
appropriate free list. Each file also needs `print_lists()`.
The main program and parser remain unfinished. Consequently `make all`
cannot link the three complete programs yet; this change covers Task 2 only.

### Build and check Task 2

On Linux, WSL, or an RMIT teaching server with GNU Make and g++:

```sh
make test-task2
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
