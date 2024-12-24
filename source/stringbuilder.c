/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * String building utilities
 */

#include "string.h"
#include "memory.h"

#include "stringbuilder.h"

void DgStringBuilderInit(DgStringBuilder *this) {
	DgMemoryZero(this, sizeof *this);
}

void DgStringBuilderAppendBytes(DgStringBuilder *this, size_t size, const void *data) {
	/**
	 * Append any bytes to the building up string
	 * 
	 * @param this String builder
	 * @param size Size of data to append
	 * @param data Data to append
	 */
	
	if (this->head + size < DG_STRING_BUILDER_LOCAL_LENGTH) {
		DgMemoryCopy(size, data, this->local + this->head);
		this->head += size;
	}
}

void DgStringBuilderAppend(DgStringBuilder *this, const char *data) {
	/**
	 * Append a string to the building up string
	 * 
	 * @param this String builder
	 * @param data Data to append to string
	 */
	
	DgStringBuilderAppendBytes(this, DgStringLength(data), data);
}

const char *DgStringBuilderGetTemp(DgStringBuilder *this) {
	/**
	 * Get a temp copy of the built up string which does not need freeing.
	 * 
	 * @param this String builder
	 * @return Built up string (pointer to string builder data)
	 */
	
	return this->local;
}

char *DgStringBuilderGet(DgStringBuilder *this) {
	/**
	 * Get a copy of the built up string in the heap
	 * 
	 * @param this String builder
	 * @return Built up string (on the heap)
	 */
	
	return DgStringDuplicate(this->local);
}
