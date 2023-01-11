/**
 * Copyright (C) 2021 - 2023 Knot126 and Descentix Software
 * ========================================================
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
