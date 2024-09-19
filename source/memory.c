/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * Memory Allocation
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "error.h"

#include "memory.h"

DgMemoryErrorHandler gMemoryErrorFunc;

void *DgMemoryAllocate(size_t size) {
	/**
	 * Allocate some memory, or return NULL on failure.
	 * 
	 * @param size Size of the memory block to alloate
	 * @return Pointer to the allocated memory, or NULL if failed
	 */
	
	void *block = malloc(size);
	
	if (gMemoryErrorFunc && !block) {
		return gMemoryErrorFunc(NULL, NULL);
	}
	
	return block;
}

DgError DgMemoryFree(void *block) {
	/**
	 * Free a block of memory
	 * 
	 * @note Same as DgFree except it handles NULL gracefully.
	 * 
	 * @param block Block of memory to free
	 */
	
	if (block == NULL) {
		return DG_ERROR_NOT_SAFE;
	}
	
	free(block);
	
	return DG_ERROR_SUCCESS;
}

void *DgMemoryReallocate(void* block, size_t size) {
	/**
	 * Reallocate a block of memory
	 * 
	 * @note Same as DgRealloc but block == NULL and size == 0 cases are handled
	 * in a sane way. I'm not sure what the C standard people were smoking for
	 * C23, but it must have given them brain damage.
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
	else {
		void *block = realloc(block, size);
		
		if (gMemoryErrorFunc && !block) {
			return gMemoryErrorFunc(NULL, NULL);
		}
		
		return block;
	}
}

void DgMemorySetErrorHandler(DgMemoryErrorHandler handler) {
	/**
	 * Set the optional global memory error handler function to `handler`.
	 * 
	 * @param handler Function which handles memory errors
	 */
	
	gMemoryErrorFunc = handler;
}

void *DgMemoryCopy(size_t length, const void *from, void *to) {
	/**
	 * Move `length` bytes of memory upstream or downstream in `from` to `to`.
	 * 
	 * @note This is like memmove, not memcpy
	 * 
	 * @param length Amount of memory to move
	 * @param from Where to copy from
	 * @param to Where to put the copied memory
	 * @return `to` or NULL if failed
	 */
	
	return memmove(to, from, length);
}

bool DgMemoryEqual(size_t length, const void *block1, const void *block2) {
	/**
	 * Compare the first length bytes of the contents that block1 and block2
	 * point to.
	 * 
	 * @note Some edge cases are defined:
	 * 
	 * 1. If length is zero, always return true.
	 * 2. If length is nonzero and block1 and/or block2 is NULL, then return false.
	 * 
	 * @param length Length of both blocks
	 * @param block1 First memory block
	 * @param block2 Second memory block
	 * @return true if *block1 == *block2 for length bytes of data
	 */
	
	if (!length) {
		return true;
	}
	
	if (!block1 || !block2) {
		return false;
	}
	
	return !memcmp(block1, block2, length);
}

void DgMemoryZero(const void *block, size_t length) {
	/**
	 * Fill the memory starting at block with zero bytes.
	 */
	
	memset((void *) block, 0, length);
}

void DgMemorySwapEndian(void *block, size_t length) {
	/**
	 * Swap the endianness of the given block.
	 * 
	 * @note This code is ugly but that's more because of the signature being
	 * (void *) and not (uint8_t *).
	 * 
	 * @warning This function may be a macro in a future implementation.
	 * 
	 * @param block Block of memory to swap
	 * @param length Length of block
	 */
	
	uint8_t *block_ = (uint8_t *) block;
	size_t half_len = length / 2;
	
	for (size_t i = 0; i < half_len; i++) {
		uint8_t temp = block_[i];
		block_[i] = block_[length - 1 - i];
		block_[length - 1 - i] = temp;
	}
}
