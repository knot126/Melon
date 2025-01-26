/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Generic, bounds checked value arrays
 */

#include "error.h"
#include "memory.h"
#include "array.h"
#include "log.h"
#include "value.h"

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
		this->items = DgMemoryReallocate(this->items, sizeof *this->items * new_alloced);
		
		if (!this->items) {
			return DG_ERROR_ALLOCATION_FAILED;
		}
		
		this->allocated = new_alloced;
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

DgError DgArrayResize(DgArray *this, size_t size) {
	/**
	 * Resize the array to match the requested size. If the requested size is
	 * greater than the current length, the empty slots are filled with nil.
	 * 
	 * @param this Array to resize
	 * @param size New size of the array
	 */
	
	// Prepare for the new elements
	DgError error;
	
	if ((error = DgArrayPrepareForElements(this, size - this->length))) {
		return error;
	}
	
	// Save the old length
	const size_t old_length = this->length;
	
	// Set the array length to new length
	this->length = size;
	
	// Set any extras to nil
	for (size_t i = old_length; i < size; i++) {
		// HACK: This can't fail (unless I made some off by one error), but it
		// returns an error code which for correctness really ought to be
		// checked.
		DgValue nil = DgMakeNil();
		DgArrayPut(this, i, &nil);
	}
	
	return DG_ERROR_SUCCESS;
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

DgError DgArrayRemoveND(DgArray *this, size_t index, size_t amount, bool deep) {
	/**
	 * Remove `amount` elements from the array `this` starting at `index`.
	 * 
	 * @param this Array to remove from
	 * @param index Starting index of elements to remove
	 * @param amount Number of elements to remove
	 * @param deep If the elements should be freed or not
	 * @return DG_ERROR_OUT_OF_RANGE if index + amount - 1 is not a valid index in the array,
	 * DG_ERROR_SUCCESS if removing items are successful
	 */
	
	if (!DgArrayInRange(this, index + amount - 1)) {
		return DG_ERROR_OUT_OF_RANGE;
	}
	
	if (deep) {
		for (size_t i = index; i < index + amount; i++) {
			DgValueFree(&this->items[i]);
		}
	}
	
	DgMemoryCopy(sizeof *this->items * ((this->length - index) - amount), &this->items[index + amount], &this->items[index]);
	
	this->length -= amount;
	
	return DG_ERROR_SUCCESS;
}

DgError DgArrayRemove(DgArray *this, size_t index) {
	/**
	 * Remove one element at the given index.
	 */
	
	return DgArrayRemoveND(this, index, 1, false);
}

DgError DgArrayConcat(DgArray *this, DgArray *other) {
	/**
	 * Append the raw contents of `other` onto `this`
	 */
	
	return DG_ERROR_NOT_IMPLEMENTED;
}

bool DgArrayEqual(DgArray *this, DgArray *other) {
	/**
	 * Compare the current array with another array.
	 * 
	 * @param this First array
	 * @param other Other array
	 * @return true if arrays are equal, false if not
	 */
	
	size_t this_len = DgArrayLength(this), other_len = DgArrayLength(other);
	
	if (this_len != other_len) {
		return false;
	}
	
	for (size_t i = 0; i < this_len; i++) {
		if (!DgValueEqual(DgArrayAt(this, i), DgArrayAt(other, i))) {
			return false;
		}
	}
	
	return true;
}
