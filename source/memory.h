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

#ifdef DG_MELON_OLD_MEMORY_FUNCTIONS
void *DgAlloc(size_t size);
void DgFree(void *block);
void *DgRealloc(void *block, size_t size);
#endif

void *DgMemoryAllocate(size_t size);
DgError DgMemoryFree(void *block);
void *DgMemoryReallocate(void *block, size_t size);

void *DgMemoryCopy(size_t length, const void *from, void *to);
bool DgMemoryEqual(size_t length, const void *block1, const void *block2);
void DgMemoryZero(const void *block, size_t length);
