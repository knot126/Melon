/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Arrays packed for specific types
 */

#pragma once

#include "common.h"

/**
 * The main array structure
 */
typedef struct DgPackedArray {
	void *items;
	size_t length;
	size_t capacity;
	size_t item_size;
} DgPackedArray;

void DgPackedArrayInit(DgPackedArray *this, size_t item_size);
