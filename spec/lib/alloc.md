# Module `alloc`

## Types and Structures

*There are no structures in this header.*

## Functions

### `void *DgAlloc(size_t size);`

> This function is considered deprecated, and may not be implemented.

This function MUST return an allocated block of memory of at least size `size`. If there is an error and the memory block cannot be allocated, then the function MUST return `NULL`.

### `void DgFree(void *block);`

> This function is considered deprecated, and may not be implemented.

This function MUST free a memory block.

### `void *DgRealloc(void *block, size_t size);`

> This function is considered deprecated, and may not be implemented.

This function MUST reallocate a memory block to be at least the size `size`. It shall have the same behaviours as the C function `realloc`.

### `void *DgMemoryAllocate(size_t size);`

This function MUST return an allocated block of memory of at least size `size`. If there is an error and the memory block cannot be allocated, then the function MUST return `NULL`.

### `void DgMemoryFree(void *block);`

This function MUST free a memory block.

### `void *DgMemoryReallocate(void *block, size_t size);`

This function MUST reallocate a memory block to be at least the size `size`. It shall have the same behaviours as the C function `realloc`, except that when `size == 0`, the function MUST free the memory block and return `NULL`.

## [Prev: Conventions](conventions.md)
## [Prev: Module `args`](args.md)
