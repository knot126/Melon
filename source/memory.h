/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * Memory Allocation
 */

#pragma once

#include <stdlib.h>
#include "error.h"

/* Global allocator */
void *DgMemoryAllocate(size_t size);
DgError DgMemoryFree(void *block);
void *DgMemoryReallocate(void *block, size_t size);

/* Global allocator error handler */
enum {
	DG_MEMORY_ALLOC_ERROR_INFO_FUNC_ALLOC = 1,
	DG_MEMORY_ALLOC_ERROR_INFO_FUNC_REALLOC,
};

typedef struct DgMemoryErrorInfo {
	void *old_block;
	size_t size;
	uint8_t function;
} DgMemoryErrorInfo;

typedef void *(*DgMemoryErrorHandler)(const void *context, const void *error_info);

void DgMemorySetErrorHandler(DgMemoryErrorHandler handler);

/* Common operations */
void *DgMemoryCopy(size_t length, const void *from, void *to);
bool DgMemoryEqual(size_t length, const void *block1, const void *block2);
void DgMemoryZero(const void *block, size_t length);

void DgMemorySwapEndian(void *block, size_t length);
