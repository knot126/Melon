/*
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * Memory Allocation
 */

#pragma once

#include <stdlib.h>

void *DgAlloc(size_t size);
void DgFree(void *block);
void *DgRealloc(void *block, size_t size);

void *DgMemoryAllocate(size_t size);
void DgMemoryFree(void *block);
void *DgMemoryReallocate(void *block, size_t size);
