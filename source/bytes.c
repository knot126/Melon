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

#include <stdlib.h>

#include "alloc.h"
#include "log.h"

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

DgByte DgBytesAt_(DgBytes *this, size_t index, const char *debug_file, size_t debug_line) {
	/**
	 * Get the byte at the given index, panicing the process if the index
	 * exceedes the boundary.
	 * 
	 * @param this Bytes object
	 * @param index Index to get
	 * @return Byte or a dead process :3
	 */
	
	if (index >= this->length) {
		DgLog(DG_LOG_FATAL, "Out of bounds read on DgBytes object: <0x%x>[0x%x]\n\tFile: %s \n\tLine: %d", this->data, index, debug_file, debug_line);
		exit(-1);
	}
	
	return this->data[index];
}

void DgBytesSet_(DgBytes *this, size_t index, DgByte byte, const char *debug_file, size_t debug_line) {
	/**
	 * Set the byte at the given index, or panic if OOB.
	 * 
	 * @param this Bytes object
	 * @param index Index to get
	 * @return Byte or a dead process :3
	 */
	
	if (index >= this->length) {
		DgLog(DG_LOG_FATAL, "Out of bounds write on DgBytes object: <0x%x>[0x%x]\n\tFile: %s \n\tLine: %d", this->data, index, debug_file, debug_line);
		exit(-1);
	}
	
	this->data[index] = byte;
}