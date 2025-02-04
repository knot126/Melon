/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Arrays packed for specific types
 */

#include "memory.h"

#include "packedarray.h"

void DgPackedArrayInit(DgPackedArray *this, size_t item_size) {
	/**
	 * Initialise a packed array with items of the given size in bytes
	 * 
	 * @param this Packed array object
	 * @param item_size Size of each item
	 */
	
	DgMemoryZero(this, sizeof *this);
	this->item_size = item_size;
}
