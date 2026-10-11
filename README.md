# Project 2 - Memory Allocation

COSC1114 Operating Systems Principles. C++ memory allocation simulator
implementing First Fit, Best Fit, and Quick Fit allocation strategies.

## Project layout

```text
Project2_OS_Assignment/
|-- include/
|   |-- allocation_core.h       # Shared allocation record and bookkeeping
|   |-- allocation_strategy.h   # Allocation strategy interface
|   |-- chunk.h                 # Chunk allocation helpers
|   |-- memory.h                # Public alloc/dealloc interface
|   `-- parser.h                # Datafile parser interface
|
|-- src/
|   |-- firstfit.cpp            # First Fit implementation
|   |-- bestfit.cpp             # Best Fit implementation
|   |-- quickfit.cpp            # Quick Fit implementation
|   |-- chunk.cpp               # Partition rounding and sbrk() handling
|   |-- parser.cpp              # Reads alloc/dealloc commands
|   `-- main.cpp                # Program entry point
|
|-- tests/
|   |-- task2_test.cpp
|   |-- heap_failure_test.cpp
|   |-- dealloc_firstfit_test.cpp
|   |-- dealloc_bestfit_test.cpp
|   `-- dealloc_quickfit_test.cpp
|
|-- Makefile
|-- p2_gen.sh                   # Alloc/dealloc sequence generator
|-- datafile                    # Sample input
`-- README.md
```

Run all commands below from the project root.

## Build

Build all three allocation programs with:

```sh
make all
```

or:

```sh
make
```

The Makefile compiles the shared source files and links them with each
allocation strategy to create:

```text
firstfit
bestfit
quickfit
```

The intermediate object and dependency files are stored in `build/`.

Build an individual strategy with:

```sh
make firstfit
make bestfit
make quickfit
```

Remove generated executables and build files with:

```sh
make clean
```

To rebuild everything from scratch:

```sh
make clean
make all
```

## Running

Each program takes a datafile containing allocation and deallocation commands.

```sh
./firstfit datafile
./bestfit datafile
./quickfit datafile
```

Example input:

```text
alloc: 20
alloc: 100
alloc: 250
dealloc
alloc: 60
```

`alloc: N` requests `N` bytes.

`dealloc` deallocates the most recently allocated chunk that has not already
been deallocated.

After processing the complete datafile, the program prints the allocated and
free memory lists.

## Memory allocation

Memory is allocated using fixed partition sizes:

```text
32, 64, 128, 256, 512 bytes
```

Requests are rounded up to the smallest partition that can contain them.

First Fit selects the first suitable free chunk.

Best Fit selects the smallest suitable free chunk.

Quick Fit maintains a separate free list for each partition size.

It checks the requested size class first, then larger classes before growing
the heap. Reused chunks retain their full partition size; they are not split.
All strategies keep their lists private to their implementation file and
delete accounting records at shutdown without returning chunk storage to the OS.

Deallocated chunks are returned to the appropriate free list for later reuse
rather than being returned to the operating system.

## Generate a datafile

The supplied `p2_gen.sh` script generates a sequence of `alloc` and `dealloc`
commands.

Make the script executable:

```sh
chmod +x p2_gen.sh
```

Generate 20 operations:

```sh
./p2_gen.sh 20 > datafile
```

View the generated input:

```sh
cat datafile
```

Then run any strategy using the generated file:

```sh
./firstfit datafile
```

## Tests

Build all three programs and run all allocation/strategy/deallocation tests:

```sh
make test
```

Strategy tests use the public API and cover non-LIFO deallocation, First Fit
versus Best Fit selection, Quick Fit larger-class reuse, FIFO free-list order,
and fatal rejection of unknown, interior, null, and double-freed pointers.
Fatal-error messages during these tests are expected from child processes.
The full test target also needs Python 3 for command-line input checks and a
repeatable 3,000-operation workload for each strategy.

Run the memory allocation tests with:

```sh
make test-task2
```

Run the deallocation tests with:

```sh
make test-task4
```

The tests cover partition allocation, invalid requests, allocation failure,
free-chunk reuse, and movement of deallocated chunks back into the appropriate
free lists.

## Compiler settings

The project is compiled using C++11 with:

```text
-std=c++11 -Wall -Werror
```

The allocator uses the Linux `sbrk()` system call for requesting new heap
memory.
