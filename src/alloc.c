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

void *DgMemoryAllocate(size_t size) {
	/**
	 * Allocate some memory, or return NULL on failure.
	 * 
	 * @param size Size of the memory block to alloate
	 * @return Pointer to the allocated memory, or NULL if failed
	 */
	
	return DgAlloc(size);
}

void DgMemoryFree(void *block) {
	/**
	 * Free a block of memory
	 * 
	 * @note Same as DgFree except it handles NULL gracefully.
	 * 
	 * @param block Block of memory to free
	 */
	
	if (block == NULL) {
		return;
	}
	
	DgFree(block);
}

void *DgMemoryReallocate(void* block, size_t size) {
	/**
	 * Reallocate a block of memory
	 * 
	 * @note Same as DgRealloc but block == NULL and size == 0 cases are handled
	 * so legacy C enviornments still have the modern behaviour.
	 * 
	 * @param block Block of memory to reallocate, or NULL if allocating new memory
	 * @param size Size of the reallocated memory block
	 * @return Reallocated block of memory, or NULL if failed
	 */
	
	if (size == 0) {
		DgMemoryFree(block);
		return NULL;
	}
	else if (block == NULL) {
		return DgMemoryAllocate(size);
	}
	
	return DgRealloc(block, size);
}
