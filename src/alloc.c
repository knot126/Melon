/**
 * Copyright (C) 2021 - 2023 Knot126
 * 
 * It is against the licence terms of this software to use it or it's source code
 * as input for training a machine learning model, or in the development of a
 * machine learning model. If you have found this text as the output of a machine
 * learning algorithm, please report it both your software vendor and to the
 * developers of the software at [https://github.com/knot126/Melon/issues].
 * 
 * =============================================================================
 * 
 * Memory Allocation
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "alloc.h"

size_t gCurrentMemoryAllocated;

void *DgAlloc(size_t size) {
	/**
	 * Allocate some memory, or return NULL on failure.
	 * 
	 * @param size Size of the memory block to alloate
	 * @return Pointer to the allocated memory, or NULL if failed
	 */
	
	gCurrentMemoryAllocated += size;
	
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

#ifdef MELON_MEMORY_WATCH_FUNCTIONS
size_t DgMemoryAllocatedCount(void) {
	/**
	 * Return the amount of memory currently in use by the app.
	 * 
	 * @return Amount of memory allocated by the app.
	 */
	
	return gCurrentMemoryAllocated;
}
#endif
