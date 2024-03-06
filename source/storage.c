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
 * Storage facilities
 */

#include "common.h"

#include "error.h"
#include "alloc.h"
#include "string.h"
#include "log.h"

#include "storage.h"

/**
 * Default storage config
 */

static DgStorage *gGlobalStorageConfig_;

void DgStorageSetDefault(DgStorage *this) {
	/**
	 * Setter for default storage object
	 * 
	 * @param this New default storage object
	 */
	
	gGlobalStorageConfig_ = this;
}

DgStorage *DgStorageGetDefault(void) {
	/**
	 * Getter for default storage object
	 * 
	 * @return Current default storage object
	 */
	
	return gGlobalStorageConfig_;
}

static DgStorage *DgStorageInitGlobal(void) {
	/**
	 * Initialise the default storage config
	 * 
	 * @return Default storage config (or NULL if failed to init)
	 */
	
	gGlobalStorageConfig_ = DgAlloc(sizeof *gGlobalStorageConfig_);
	
	if (!gGlobalStorageConfig_) {
		return NULL;
	}
	
	DgStorageInit(gGlobalStorageConfig_);
	
	return gGlobalStorageConfig_;
}

static DgStorage *DgStorageResolve(DgStorage *this) {
	/**
	 * Resolve which storage configuration to use (e.g. if NULL, use default,
	 * otherwise use the given storage object)
	 * 
	 * @param this Candidate storage config object or NULL
	 * @return Storage config object to use
	 */
	
	// Return specified storage config object
	if (this) {
		return this;
	}
	// Return current global storage object
	else if (gGlobalStorageConfig_) {
		return gGlobalStorageConfig_;
	}
	// Initialise global and return
	else {
		return DgStorageInitGlobal();
	}
}

#define DG_STORAGE_RESOLVE() this = DgStorageResolve(this);\
	\
	if (!this) {\
		DgLog(DG_LOG_ERROR, "Failed to initialise storage config!");\
		return DG_ERROR_NOT_INITIALISED;\
	}\
	\
	

/**
 * Init and free
 */

DgError DgStorageInit(DgStorage *this) {
	/**
	 * Initialise the storage configuration
	 * 
	 * @note You should never have to write DgStorageInit(NULL); the default
	 * storage will be initialised automatically.
	 * 
	 * @param this Storage config object
	 * @return Error status
	 */
	
	// Resolve which storage to use
	DG_STORAGE_RESOLVE();
	
	// Zero out memory
	memset(this, 0, sizeof *this);
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgStorageFree(DgStorage *this) {
	/**
	 * Free storage configuration
	 * 
	 * @note If you are just using the default storage configuration (passing in
	 * NULL for DgStorage pointers) there isn't really a need to worry about
	 * ever calling this function unless you need to reset state. The OS can
	 * clean up after you.
	 * 
	 * @param this Storage config object
	 * @return Error status
	 */
	
	DG_STORAGE_RESOLVE();
	
	if (this->pool) {
		DgFree(this->pool);
	}
	
	return DG_ERROR_SUCCESSFUL;
}

/**
 * Pool manegement
 */

DgError DgStorageAddPool(DgStorage *this, DgStoragePool *pool) {
	/**
	 * Add the given pool to the given storage configuration
	 * 
	 * @warning This will automatically free the memory at `pool` since it is
	 * temporary anyways.
	 * 
	 * @note The reason we do it like this is so that it makes creating one-liner
	 * storage configurations very easy.
	 * 
	 * @param this Storage configuration
	 * @param pool Pool to add to the storage configuration
	 * @return Error code
	 */
	
	// Check for null
	if (pool == NULL) {
		return DG_ERROR_NOT_SAFE;
	}
	
	DG_STORAGE_RESOLVE();
	
	// Check if the pool already exists
	if (DgStorageHasPool(this, pool->protocol)) {
		return DG_ERROR_ALREADY_EXISTS;
	}
	
	// Allocate the pool's memory
	this->pool_count++;
	this->pool = DgMemoryReallocate(this->pool, sizeof *this->pool * this->pool_count);
	
	if (!this->pool) {
		return DG_ERROR_ALLOCATION_FAILED;
	}
	
	// Copy pool data
	this->pool[this->pool_count - 1] = *pool;
	
	// Clear temporary pool memory
	DgFree(pool);
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgStorageRemovePool(DgStorage *this, DgStoragePath protocol) {
	/**
	 * Remove the storage pool identified by `protocol`.
	 * 
	 * @note Any memory assocaited with the DgStoragePool object is automatically
	 * freed. Do not try to free it yourself!
	 * 
	 * @note Not currently implemented and not a priority because there is not
	 * much of a need to remove storage configurations. (This would only really
	 * work with game mods.)
	 * 
	 * @param this Storage configuration
	 * @param protocol The protocol of the pool to be removed
	 * @return Error code
	 */
	
	DG_STORAGE_RESOLVE();
	
	return DG_ERROR_NOT_IMPLEMENTED;
}

DgError DgStorageGetPool(DgStorage *this, DgStoragePath protocol, DgStoragePool **pool) {
	/**
	 * Get a pointer to the storage pool given the protocol.
	 * 
	 * @warning There is generally no need to use this function!
	 * 
	 * @param this Storage configuration
	 * @param protocol The protocol of the pool to retrieve
	 * @param pool Pointer to the pointer that will store the address of the pool object (or NULL)
	 * @return Error code
	 */
	
	DG_STORAGE_RESOLVE();
	
	for (size_t index = 0; index < this->pool_count; index++) {
		if (DgStringEqual(this->pool[index].protocol, protocol)) {
			if (pool) {
				pool[0] = &this->pool[index];
			}
			
			return DG_ERROR_SUCCESSFUL;
		}
	}
	
	return DG_ERROR_NOT_FOUND;
}

bool DgStorageHasPool(DgStorage *this, DgStoragePath protocol) {
	/**
	 * Return `true` if the given storage configuration has a pool identified by `protocol`.
	 * Otherwise, return `false`.
	 * 
	 * @param this Storage configuration
	 * @param path Path to the file to delete (including protocol)
	 * @return True if the storage configuration has a pool by the name `protocol`
	 */
	
	return (DgStorageGetPool(this, protocol, NULL) == DG_ERROR_SUCCESSFUL);
}

DgError DgStorageRenamePool(DgStorage *this, DgStoragePath old_protocol, DgStoragePath new_protocol) {
	/**
	 * Change the protocol of the pool identified by `old_protocol` to `new_protocol`.
	 * 
	 * @param this Storage configuration
	 * @param old_protocol Current protocol name
	 * @param new_protocol New protocol name
	 * @return Error code
	 */
	
	DG_STORAGE_RESOLVE();
	
	return DG_ERROR_NOT_IMPLEMENTED;
}

/**
 * Utility functions for type implementations
 */
DgError DgStorageSplitPathIntoParts(DgStoragePath path, char **protocol, char **filename) {
	/**
	 * Splits a path into the protocol and the filename.
	 * 
	 * @param path Path to the file
	 * @param protocol Where to store the resulting potocol string (or NULL)
	 * @param filename Where to store the resulting file name (or NULL)
	 * @return Error code
	 */
	
	int64_t seperator_index = DgStringFind(path, "://", 0);
	
	if (seperator_index == -1) {
		return DG_ERROR_FILE_NOT_FOUND;
	}
	
	// Store the protocol string
	if (protocol) {
		char *protocol_ = DgStringDuplicateUntil(path, seperator_index);
		
		if (!protocol_) {
			return DG_ERROR_OUT_OF_MEMORY;
		}
		
		protocol[0] = protocol_;
	}
	
	// Store the other part of the path
	if (filename) {
		char *filename_ = DgStringDuplicate(&path[seperator_index + 3]);
		
		if (!filename_) {
			return DG_ERROR_OUT_OF_MEMORY;
		}
		
		filename[0] = filename_;
	}
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgStorageGetPoolFromPath(DgStorage *this, DgStoragePath path, DgStoragePool **pool) {
	/**
	 * Get the storage pool assocaited with the given path.
	 * 
	 * @param this Storage config object
	 * @param path Path to use to find the pool
	 * @param pool Where to store the found pool
	 * @return Error code
	 */
	
	char *protocol;
	
	// Get the protocol
	DgError status = DgStorageSplitPathIntoParts(path, &protocol, NULL);
	
	if (status) {
		return status;
	}
	
	// Find the pool from the protocol
	status = DgStorageGetPool(this, protocol, pool);
	
	if (status) {
		return status;
	}
	
	// Free temporary memory
	DgFree(protocol);
	
	return DG_ERROR_SUCCESSFUL;
}

/**
 * File system functions
 */

DgError DgStorageDelete(DgStorage *this, DgStoragePath path) {
	/**
	 * Delete the file at the specified path.
	 * 
	 * @param this Storage configuration
	 * @param path Path to the file to delete (including protocol)
	 * @return Error code
	 */
	
	DG_STORAGE_RESOLVE();
	
	// Find the pool
	DgStoragePool *pool;
	
	DgError status = DgStorageGetPoolFromPath(this, path, &pool);
	
	if (status) {
		return status;
	}
	
	// Call its function
	return pool->functions->delete(this, pool, path);
}

DgError DgStorageRename(DgStorage *this, DgStoragePath old_path, DgStoragePath new_path) {
	/**
	 * Rename the file at `old_path` to `new_path`. This should overwrite the file
	 * at `new_path` if it already exists.
	 * 
	 * @note This will automatically create any parent folders nessicary.
	 * 
	 * @note Generally, you should try to avoid renaming files unless the user has
	 * explicitly asked you to do so.
	 * 
	 * @warning You cannot currently change the pool that a file is in.
	 * 
	 * @param this Storage configuration
	 * @param old_path Old file path
	 * @param new_path New file path
	 * @return Error code
	 */
	
	DG_STORAGE_RESOLVE();
	
	// Find the pool
	DgStoragePool *pool;
	
	DgError status = DgStorageGetPoolFromPath(this, old_path, &pool);
	
	if (status) {
		return status;
	}
	
	// Call its function
	return pool->functions->rename(this, pool, old_path, new_path);
}

DgError DgStorageCreateFile(DgStorage *this, DgStoragePath path) {
	/**
	 * Create a zero-byte file at `path`. This will overwrite the file if it
	 * already exists. If this is an existing folder, it returns an error.
	 * 
	 * @note This will automatically create any parent folders nessicary.
	 * 
	 * @note Use in similar situations as "$ touch {path}"
	 * 
	 * @param this Storage configuration
	 * @param path Path to the file to create (including protocol)
	 * @return Error code
	 */
	
	DG_STORAGE_RESOLVE();
	
	// Find the pool
	DgStoragePool *pool;
	
	DgError status = DgStorageGetPoolFromPath(this, path, &pool);
	
	if (status) {
		return status;
	}
	
	// Call its function
	return pool->functions->create_file(this, pool, path);
}

DgError DgStorageCreateFolder(DgStorage *this, DgStoragePath path) {
	/**
	 * Create a folder at `path`. This will do nothing if the folder already
	 * exists. This will error if a file already exists at this path.
	 * 
	 * @note This will automatically create any parent folders nessicary.
	 * 
	 * @param this Storage configuration
	 * @param path Path to the folder to create (including protocol)
	 * @return Error code
	 */
	
	DG_STORAGE_RESOLVE();
	
	// Find the pool
	DgStoragePool *pool;
	
	DgError status = DgStorageGetPoolFromPath(this, path, &pool);
	
	if (status) {
		return status;
	}
	
	// Call its function
	return pool->functions->create_folder(this, pool, path);
}

DgStorageObjectType DgStorageType(DgStorage *this, DgStoragePath path, DgStorageObjectType *type) {
	/**
	 * Return the type of the object at `path`.
	 * 
	 * @param this Storage configuration
	 * @param path Path of the file to check (including protocol)
	 * @return Type of object
	 */
	
	DG_STORAGE_RESOLVE();
	
	// Find the pool
	DgStoragePool *pool;
	
	DgError status = DgStorageGetPoolFromPath(this, path, &pool);
	
	if (status) {
		return status;
	}
	
	// Call its function
	return pool->functions->type(this, pool, path, type);
}

DgError DgStoragePoolFree(DgStoragePool *pool) {
	/**
	 * Release memory assocaited with a storage pool.
	 * 
	 * @param pool The pool to free
	 * @return Errors encountered freeing pool
	 */
	
	if (pool->protocol) {
		DgFree((void *) pool->protocol);
	}
	
	if (pool == NULL) {
		return DG_ERROR_NOT_SAFE;
	}
	
	if (!pool->functions->free_specific_config) {
		return DG_ERROR_SUCCESSFUL;
	}
	
	return pool->functions->free_specific_config(pool);
}

/* Stream functions and similar */

DgError DgStreamOpen(DgStorage *this, DgStream *context, DgStoragePath path, DgStorageFlags flags) {
	/**
	 * Open a stream
	 * 
	 * @param this Storage object to use
	 * @param stream Where to store the resulting stream object
	 * @param path Path to the stream source to open
	 * @param flags Properties of the stream
	 * @return Error code
	 */
	
	DG_STORAGE_RESOLVE();
	
	// Find the pool
	DgStoragePool *pool;
	
	DgError status = DgStorageGetPoolFromPath(this, path, &pool);
	
	if (status) {
		return status;
	}
	
	// Pre-set the stream storage and pool
	context->storage = this;
	context->pool = pool;
	
	// Call its function
	return pool->functions->open(this, pool, context, path, flags);
}

DgError DgStreamClose(DgStream *context) {
	/**
	 * Close the file stream
	 * 
	 * @param context Stream object
	 * @return Error code
	 */
	
	DgStorage *this = context->storage;
	DgStoragePool *pool = context->pool;
	
	return pool->functions->close(this, pool, context);
}

DgError DgStreamRead(DgStream *context, size_t size, void *buffer) {
	/**
	 * Read a buffer from the file stream
	 * 
	 * @param context Stream object
	 * @param size Size of the buffer
	 * @param buffer Pointer to where to store the data
	 * @return Error code
	 */
	
	DgStorage *this = context->storage;
	DgStoragePool *pool = context->pool;
	
	return pool->functions->read(this, pool, context, size, buffer);
}

DgError DgStreamWrite(DgStream *context, size_t size, void *buffer) {
	/**
	 * Write a buffer to the file stream
	 * 
	 * @param context Stream object
	 * @param size Size of the buffer
	 * @param buffer Pointer to where to store the data
	 * @return Error code
	 */
	
	DgStorage *this = context->storage;
	DgStoragePool *pool = context->pool;
	
	return pool->functions->write(this, pool, context, size, buffer);
}

DgError DgStreamGetPosition(DgStream *context, size_t *position) {
	/**
	 * Get the position in the file stream.
	 * 
	 * @param context Stream object
	 * @param position Where to put the position
	 * @return Error code
	 */
	
	DgStorage *this = context->storage;
	DgStoragePool *pool = context->pool;
	
	return pool->functions->get_position(this, pool, context, position);
}

DgError DgStreamSetPosition(DgStream *context, size_t position) {
	/**
	 * Get the position in the file stream.
	 * 
	 * @param context Stream object
	 * @param position The position to go to
	 * @return Error code
	 */
	
	DgStorage *this = context->storage;
	DgStoragePool *pool = context->pool;
	
	return pool->functions->set_position(this, pool, context, position);
}

DgError DgStreamSeek(DgStream *context, DgStorageSeekBase base, int64_t offset) {
	/**
	 * Get the position in the file stream.
	 * 
	 * @param context Stream object
	 * @param base Seek base (current pos, start, end)
	 * @param offset The offset from the base
	 * @return Error code
	 */
	
	DgStorage *this = context->storage;
	DgStoragePool *pool = context->pool;
	
	return pool->functions->seek(this, pool, context, base, offset);
}

// Undefine resolve macro just to be clean
#undef DG_STORAGE_RESOLVE

/* Extra and conviecne functions */

size_t DgStreamLength(DgStream *context) {
	/**
	 * Get the length of a file.
	 * 
	 * @param context Stream object
	 * @return Size of the file
	 */
	
	size_t old_pos, length;
	DgError status;
	
	// We store the old position so we can later restore it
	status = DgStreamGetPosition(context, &old_pos);
	
	if (status) {
		return 0;
	}
	
	status = DgStreamSeek(context, DG_STORAGE_SEEK_END, 0);
	
	if (status) {
		return 0;
	}
	
	status = DgStreamGetPosition(context, &length);
	
	if (status) {
		return 0;
	}
	
	status = DgStreamSetPosition(context, old_pos);
	
	if (status) {
		return 0;
	}
	
	return length;
}

// Some extra storage functions that are automatically generated and for which
// there are too many of to put in this file directly.
#include "storage_generated.part"

DgError DgStreamWriteString(DgStream * restrict context, const char * restrict data) {
	/**
	 * Write a string to a stream.
	 * 
	 * @param context Stream object
	 * @param data The string to write
	 * @return Error code
	 */
	
	return DgStreamWrite(context, DgStringLength(data), data);
}

DgError DgStreamWriteIntegerString(DgStream * restrict context, int64_t data) {
	/**
	 * Write a stringified integer to a stream WITHOUT a null byte.
	 * 
	 * @param context Stream object
	 * @param data The string to write
	 * @return Error code
	 */
	
	const char *str = DgIntegerToString(10, data);
	
	if (!str) {
		return DG_ERROR_ALLOCATION_FAILED;
	}
	
	return DgStreamWrite(context, DgStringLength(str), str);
}
