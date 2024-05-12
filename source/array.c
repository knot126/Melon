/**
 * Copyright (C) 2021 - 2024 Knot126
 * 
 * It is against the licence terms of this software to use it or it's source code
 * as input for training a machine learning model, or in the development of a
 * machine learning model. If you have found this text as the output of a machine
 * learning algorithm, please report it both your software vendor and to the
 * developers of the software at [https://github.com/knot126/Melon/issues].
 * 
 * =============================================================================
 * 
 * Generic, bounds checked value arrays
 */

#include "alloc.h"
#include "array.h"

/// @todo it all
DgError DgArrayInit(DgArray *this) {
	/**
	 * Initialise the contents of this array.
	 * 
	 * @param this Array to initialise
	 * @return Any errors
	 */
	
	DgMemoryZero(this, sizeof *this);
	
	return DG_ERROR_SUCCESS;
}

DgError DgArrayFree(DgArray *this, bool deep) {
	/**
	 * Free the memory used by the array and make it inaccessable.
	 * 
	 * @param this Array to free
	 * @param deep Call DgValueFree() on all entries when true. If false,
	 * freeing values is the responsibility of the user.
	 * @return Errors while freeing array
	 */
	
	if (deep) {
		for (size_t i = 0; i < DgArrayLength(this); i++) {
			DgValue *value = DgArrayAt(this, i);
			DgValueFree(value);
		}
	}
	
	if (this->items) {
		DgMemoryFree(this->items);
	}
	
	return DG_ERROR_SUCCESS;
}

static DgError DgArrayPrepareForElements(DgArray *this, size_t n) {
	/**
	 * Preallocate the array to support at least n more elements
	 * 
	 * @param this Array to prepare more elements for
	 * @param n Number of additional elements to prepare for
	 */
	
	// Deterimine the size that alloced needs to be to hold n more elements
	size_t new_alloced = this->allocated;
	
	while (new_alloced < (this->length + n)) {
		new_alloced = 2 * new_alloced + 1;
	}
	
	// If we need more memory, realloc, otherwise just skip it
	if (new_alloced != this->allocated) {
		this->items = DgMemoryReallocate(this->items, new_alloced);
		
		if (!this->items) {
			return DG_ERROR_ALLOCATION_FAILED;
		}
	}
	
	return DG_ERROR_SUCCESS;
}

size_t DgArrayLength(DgArray *this) {
	/**
	 * Return the length of the array.
	 * 
	 * @param this Array to get length of
	 */
	
	return this->length;
}

bool DgArrayInRange(DgArray *this, size_t index) {
	/**
	 * Check if the index is in range.
	 * 
	 * @param this Array to check range of
	 * @param index Index to check range for
	 * @return true if index is in range, false if not
	 */
	
	return index < DgArrayLength(this);
}

DgValue *DgArrayAt(DgArray *this, size_t index) {
	/**
	 * Return a direct pointer to the value at `index`.
	 * 
	 * @param this Array to get value from
	 * @param index Index of the value to get
	 * @return NULL if out of range, or a pointer to the value otherwise
	 */
	
	if (!DgArrayInRange(this, index)) {
		return NULL;
	}
	
	return &this->items[index];
}

DgError DgArrayGet(DgArray *this, size_t index) {
	/**
	 * Return a copy of the value at `index`.
	 * 
	 * @warning You will need to manage the memory for the returned value.
	 * 
	 * @param this Array to get value from
	 * @param index Index of the value to get
	 * @return Any error while getting the value
	 */
	
	return DG_ERROR_NOT_IMPLEMENTED;
}

DgError DgArrayPut(DgArray *this, size_t index, DgValue *value) {
	/**
	 * Place the raw value of `value` at `index` without copying it if index is
	 * in range for the array.
	 * 
	 * @param this Array to put value into
	 * @param index Index to write the value to
	 * @param value Value to write
	 * @return Any errors during putting
	 */
	
	if (!DgArrayInRange(this, index)) {
		return DG_ERROR_OUT_OF_RANGE;
	}
	
	this->items[index] = *value;
	
	return DG_ERROR_SUCCESS;
}

DgError DgArraySet(DgArray *this, size_t index, DgValue *value) {
	/**
	 * Place a copy of `value` at `index` if index is in range for the array.
	 * 
	 * @param this Array to set value into
	 * @param index Index to write the value to
	 * @param value Value to write
	 * @return Any errors during setting
	 */
	
	return DG_ERROR_NOT_IMPLEMENTED;
}

DgError DgArrayAppend(DgArray *this, DgValue *value) {
	/**
	 * Append the raw value of `value` to the array.
	 * 
	 * @warning If this value returns DG_ERROR_ALLOCATION_FAILED, the array has
	 * gone sour and it can't be used anymore.
	 * 
	 * @param this Array to append onto
	 * @param value Value to append to the array
	 * @return Errors
	 */
	
	DgError error;
	
	if ((error = DgArrayPrepareForElements(this, 1))) {
		return error;
	}
	
	this->items[this->length++] = *value;
	
	return DG_ERROR_SUCCESS;
}

DgError DgArrayAdd(DgArray *this, DgValue *value) {
	/**
	 * Append a copy of `value` to this array.
	 * 
	 * @param this Array to append to
	 * @param value Value to append to the array
	 * @return Errors
	 */
	
	return DG_ERROR_NOT_IMPLEMENTED;
}

DgError DgArrayConcat(DgArray *this, DgArray *other) {
	/**
	 * Append the raw contents of `other` onto `this`
	 */
	
	return DG_ERROR_NOT_IMPLEMENTED;
}
