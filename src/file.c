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

#include "fs.h" // TEMP
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
	
	return DG_ERROR_NOT_IMPLEMENTED;
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
	
	static int message_shown;
	
	if (!message_shown) {
		DgLog(DG_LOG_WARNING, "DgFileSave() uses legacy file stream implementation.");
		message_shown = 1;
	}
	
	DgFileStream *file = DgFileOpen2(path, DG_FILE_STREAM_WRITE);
	DgFileStreamWrite(file, size, buffer);
	DgFileStreamClose(file);
	
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
	
	return DG_ERROR_NOT_IMPLEMENTED;
}
