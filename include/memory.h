#ifndef MEMORY_H
#define MEMORY_H

#include <cstddef>

// Returns a chunk of at least chunk_size bytes, or nullptr on failure.
// Searches the free list(s) first; calls sbrk() only if nothing fits.
void* alloc(std::size_t chunk_size);

// Moves a previously allocated chunk from the allocated list to the free
// list. Terminates the program if the pointer was never allocated.
// Does not return memory to the OS.
void dealloc(void* chunk);

// Prints the allocated list and free list(s). Called once, at the end.
void print_lists();

#endif