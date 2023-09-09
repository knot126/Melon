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
 * Byte strings
 */

#include "alloc.h"

#include "bytes.h"

void DgBytesInit(DgBytes *this) {
	/**
	 * Initialise a byte array
	 * 
	 * @param this Bytes object
	 */
	
	this->length = 0;
	this->data = NULL;
}

void DgBytesFree(DgBytes *this) {
	/**
	 * Free the given byte array
	 * 
	 * @param this Bytes object
	 */
	
	DgMemoryFree(this->data);
}

DgByte DgBytesAt_(DgBytes *this, size_t index, const char *debug_file_, size_t debug_line) {
	/**
	 * Get the byte at the given index, panicing the process if the index
	 * exceedes the boundary.
	 * 
	 * @param this Bytes object
	 * @param index Index to get
	 * @return Byte or a dead process :3
	 */
}
