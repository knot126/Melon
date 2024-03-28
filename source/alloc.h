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

#pragma once

#include <stdlib.h>
#include "error.h"

void *DgAlloc(size_t size);
void DgFree(void *block);
void *DgRealloc(void *block, size_t size);

void *DgMemoryAllocate(size_t size);
DgError DgMemoryFree(void *block);
void *DgMemoryReallocate(void *block, size_t size);

void *DgMemoryCopy(size_t length, const void *from, void *to);
bool DgMemoryEqual(size_t length, const void *block1, const void *block2);
