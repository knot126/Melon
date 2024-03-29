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
 * Load and save file functions
 */

#include "storage.h"
#include "error.h"
#include "alloc.h"
#include "log.h"

#include "file.h"

DgError DgFileLoad(DgStorage *storage, DgStoragePath path, size_t *size, void **buffer) {
	/**
	 * Load a file to a buffer.
	 * 
	 * @param storage Storage configuration
	 * @param path File path
	 * @param size Size of the buffer
	 * @param buffer Pointer to the buffer to allocate
	 * @return Error status
	 */
	
	DgStream stream;
	DgError status;
	
	status = DgStreamOpen(storage, &stream, path, DG_STREAM_READ);
	
	if (status) {
		return status;
	}
	
	size_t length = DgStreamLength(&stream);
	
	size[0] = length;
	
	buffer[0] = DgMemoryAllocate(length);
	
	if (!buffer[0]) {
		DgStreamClose(&stream);
		return DG_ERROR_ALLOCATION_FAILED;
	}
	
	status = DgStreamRead(&stream, length, buffer[0]);
	
	DgStreamClose(&stream);
	
	if (status) {
		DgFree(buffer[0]);
		return status;
	}
	
	return DG_ERROR_SUCCESS;
}

DgError DgFileSave(DgStorage *storage, DgStoragePath path, size_t size, void *buffer) {
	/**
	 * Save data to a file.
	 * 
	 * @param storage Storage configuration
	 * @param path File path
	 * @param size Size of the buffer
	 * @param buffer Buffer to save from
	 * @return Error status
	 */
	
	DgStream stream;
	DgError status;
	
	status = DgStreamOpen(storage, &stream, path, DG_STREAM_WRITE);
	
	if (status) {
		return status;
	}
	
	status = DgStreamWrite(&stream, size, buffer);
	
	if (status) {
		return status;
	}
	
	DgStreamClose(&stream);
	
	return DG_ERROR_SUCCESS;
}

DgError DgFileAppend(DgStorage *storage, DgStoragePath path, size_t size, void *buffer) {
	/**
	 * Append data to a file.
	 * 
	 * @param storage Storage configuration
	 * @param path File path
	 * @param size Size of the buffer to write
	 * @param buffer Buffer to write from
	 * @return Error status
	 */
	
	DgStream stream;
	DgError status;
	
	status = DgStreamOpen(storage, &stream, path, DG_STREAM_READ | DG_STREAM_WRITE | DG_STREAM_START_AT_END);
	
	if (status) {
		return status;
	}
	
	status = DgStreamWrite(&stream, size, buffer);
	
	DgStreamClose(&stream);
	
	if (status) {
		return status;
	}
	
	return DG_ERROR_SUCCESS;
}
