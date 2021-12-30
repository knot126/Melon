/**
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * Memory Allocation
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "alloc.h"

void *DgAlloc(size_t size) {
	/**
	 * Allocate some memory, or return NULL on failure.
	 * 
	 * @param size Size of the memory block to alloate
	 * @return Pointer to the allocated memory, or NULL if failed
	 */
	
	return malloc(size);
}

void DgFree(void *block) {
	/**
	 * Free a block of memory
	 * 
	 * @param block Block of memory to free
	 */
	
	free(block);
}

void *DgRealloc(void* block, size_t size) {
	/**
	 * Reallocate a block of memory
	 * 
	 * @param block Block of memory to reallocate, or NULL if allocating new memory
	 * @param size Size of the reallocated memory block
	 * @return Reallocated block of memory, or NULL if failed
	 */
	
	return realloc(block, size);
}
