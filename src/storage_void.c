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
 * Void storage class
 * 
 * @note This can be used as a basis for other storage classes.
 */

#include "alloc.h"
#include "string.h"
#include "storage.h"
#include "log.h"

#include "storage_void.h"

static DgError DgVoid_Rename(DgStorage *storage, DgStoragePool *pool, DgStoragePath old_path, DgStoragePath new_path) {
	/**
	 * Rename a file in this pool
	 * 
	 * @param storage Storage object
	 * @param pool Pool that the file belongs to
	 * @param old_path Old file name
	 * @param new_path New file name
	 * @return Error code
	 */
	
	DgLog(DG_LOG_VERBOSE, "DgVoid_Rename() called!! %s -> %s", old_path, new_path);
	
	return DG_ERROR_SUCCESSFUL;
}

static DgError DgVoid_Delete(DgStorage *storage, DgStoragePool *pool, DgStoragePath path) {
	/**
	 * Remove a file in this pool
	 * 
	 * @param storage Storage object
	 * @param pool Pool that the file belongs to
	 * @param path File name
	 * @return Error code
	 */
	
	DgLog(DG_LOG_VERBOSE, "DgVoid_Delete() called!! %s", path);
	
	return DG_ERROR_SUCCESSFUL;
}

static DgError DgVoid_CreateFile(DgStorage *storage, DgStoragePool *pool, DgStoragePath path) {
	/**
	 * Create an empty file in this pool. This is similar to the posix touch command.
	 * 
	 * @param storage Storage object
	 * @param pool Pool that the file belongs to
	 * @param path File name
	 * @return Error code
	 */
	
	DgLog(DG_LOG_VERBOSE, "DgVoid_CreateFile() called!! %s", path);
	
	return DG_ERROR_SUCCESSFUL;
}

static DgError DgVoid_CreateFolder(DgStorage *storage, DgStoragePool *pool, DgStoragePath path) {
	/**
	 * Create the specified folder(s)
	 * 
	 * @param storage Storage object
	 * @param pool Pool that the file belongs to
	 * @param path Folder name
	 * @return Error code
	 */
	
	DgLog(DG_LOG_VERBOSE, "DgVoid_CreateFolder() called!! %s", path);
	
	return DG_ERROR_SUCCESSFUL;
}

static DgError DgVoid_Type(DgStorage *storage, DgStoragePool *pool, DgStoragePath path, DgStorageObjectType *type) {
	/**
	 * Find the type of file at path
	 * 
	 * @param storage Storage object
	 * @param pool Pool that the file belongs to
	 * @param path File name
	 * @param type Where to put the result
	 * @return Error code
	 */
	
	DgLog(DG_LOG_VERBOSE, "DgVoid_Type() called!! %s", path);
	
	type[0] = DG_STORAGE_TYPE_REMOTE;
	
	return DG_ERROR_SUCCESSFUL;
}

static DgError DgVoid_Open(DgStorage *storage, DgStoragePool *pool, DgStream *context, DgStoragePath path, DgStorageFlags flags) {
	/**
	 * Open a file stream
	 * 
	 * @param storage Storage object
	 * @param pool Pool that the file belongs to
	 * @param stream Stream object
	 * @param path The file to open
	 * @param flags Properties of the stream
	 * @return Error code
	 */
	
	DgLog(DG_LOG_VERBOSE, "DgVoid_Open() called!!");
	
	return DG_ERROR_SUCCESSFUL;
}

static DgError DgVoid_Close(DgStorage *storage, DgStoragePool *pool, DgStream *context) {
	/**
	 * Close a file stream
	 * 
	 * @param storage Storage object
	 * @param pool Pool that the file belongs to
	 * @param stream Stream object
	 * @return Error code
	 */
	
	DgLog(DG_LOG_VERBOSE, "DgVoid_Close() called!!");
	
	return DG_ERROR_SUCCESSFUL;
}

static DgError DgVoid_Read(DgStorage *storage, DgStoragePool *pool, DgStream *context, size_t size, void *buffer) {
	/**
	 * Read from a file stream
	 * 
	 * @param storage Storage object
	 * @param pool Pool that the file belongs to
	 * @param context Stream object
	 * @param size Size of the data to read
	 * @param buffer Buffer to read into
	 * @return Error code
	 */
	
	DgLog(DG_LOG_VERBOSE, "DgVoid_Read() called!!");
	
	return DG_ERROR_SUCCESSFUL;
}

static DgError DgVoid_Write(DgStorage *storage, DgStoragePool *pool, DgStream *context, size_t size, void *buffer) {
	/**
	 * Write to a file stream
	 * 
	 * @param storage Storage object
	 * @param pool Pool that the file belongs to
	 * @param context Stream object
	 * @param size Size of the data to write
	 * @param buffer Buffer to write out
	 * @return Error code
	 */
	
	DgLog(DG_LOG_VERBOSE, "DgVoid_Write() called!!");
	
	return DG_ERROR_SUCCESSFUL;
}

static DgError DgVoid_GetPosition(DgStorage *storage, DgStoragePool *pool, DgStream *context, size_t *position) {
	/**
	 * Get the current position of a file stream
	 * 
	 * @param storage Storage object
	 * @param pool Pool that the file belongs to
	 * @param context Stream object
	 * @param position Where to put the positon
	 * @return Error code
	 */
	
	DgLog(DG_LOG_VERBOSE, "DgVoid_GetPosition() called!!");
	
	return DG_ERROR_SUCCESSFUL;
}

static DgError DgVoid_SetPosition(DgStorage *storage, DgStoragePool *pool, DgStream *context, size_t position) {
	/**
	 * Get the current position of a file stream
	 * 
	 * @param storage Storage object
	 * @param pool Pool that the file belongs to
	 * @param context Stream object
	 * @param position What to set the position to
	 * @return Error code
	 */
	
	DgLog(DG_LOG_VERBOSE, "DgVoid_SetPosition() called!!");
	
	return DG_ERROR_SUCCESSFUL;
}

static DgError DgVoid_Seek(DgStorage *storage, DgStoragePool *pool, DgStream *context, DgStorageSeekBase base, int64_t offset) {
	/**
	 * Seek to a positon in the file stream, relative to base
	 * 
	 * @param storage Storage object
	 * @param pool Pool that the file belongs to
	 * @param context Stream object
	 * @param base The base of where to start
	 * @param offset Offset after base
	 * @return Error code
	 */
	
	DgLog(DG_LOG_VERBOSE, "DgVoid_Seek() called!!");
	
	return DG_ERROR_SUCCESSFUL;
}

DgStorageFunctions gVoidFunctions = {
	.create_file = &DgVoid_CreateFile,
	.create_folder = &DgVoid_CreateFolder,
	.type = &DgVoid_Type,
	.rename = &DgVoid_Rename,
	.delete = &DgVoid_Delete,
	.open = &DgVoid_Open,
	.close = &DgVoid_Close,
	.read = &DgVoid_Read,
	.write = &DgVoid_Write,
	.get_position = &DgVoid_GetPosition,
	.set_position = &DgVoid_SetPosition,
	.seek = &DgVoid_Seek,
	.free_specific_config = NULL,
};

DgStoragePool *DgVoidCreatePool(const char *protocol) {
	/**
	 * Create an empty pool void pool
	 * 
	 * @param protocol Protocol this pool can be accessed by
	 * @return Pointer to the storage pool (or NULL if failed)
	 */
	
	DgStoragePool *pool = DgAlloc(sizeof *pool);
	
	if (!pool) {
		return NULL;
	}
	
	pool->protocol = DgStringDuplicate(protocol);
	pool->functions = &gVoidFunctions;
	pool->specific_config = NULL;
	
	return pool;
}
