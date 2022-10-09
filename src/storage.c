/**
 * Copyright (C) 2022 Knot126
 * ==========================
 * 
 * Storage facilities
 */

#include "error.h"
#include "alloc.h"
#include "log.h"

#include "storage.h"

/**
 * Default storage config
 */

DgStorage *gGlobalStorageConfig_;

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
	}

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
	
	return DG_ERROR_NOT_IMPLEMENTED;
}

DgError DgStorageRemovePool(DgStorage *this, DgStoragePath protocol) {
	/**
	 * Remove the storage pool identified by `protocol`.
	 * 
	 * @note Any memory assocaited with the DgStoragePool object is automatically
	 * freed. Do not try to free it yourself!
	 * 
	 * @param this Storage configuration
	 * @param protocol The protocol of the pool to be removed
	 * @return Error code
	 */
	
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
	 * @param pool Pointer to the pointer that will store the address of the pool object
	 * @return Error code
	 */
	
	return DG_ERROR_NOT_IMPLEMENTED;
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
	
	return false;
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
	
	return DG_ERROR_NOT_IMPLEMENTED;
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
	
	return DG_ERROR_NOT_IMPLEMENTED;
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
	 * @param this Storage configuration
	 * @param old_path Old file path
	 * @param new_path New file path
	 * @return Error code
	 */
	
	return DG_ERROR_NOT_IMPLEMENTED;
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
	
	return DG_ERROR_NOT_IMPLEMENTED;
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
	
	return DG_ERROR_NOT_IMPLEMENTED;
}

DgStorageObjectType DgStorageType(DgStorage *this, DgStoragePath path) {
	/**
	 * Return the type of the object at `path`.
	 * 
	 * @param this Storage configuration
	 * @param path Path of the file to check (including protocol)
	 * @return Type of object
	 */
	
	return DG_ERROR_NOT_IMPLEMENTED;
}

// Undefine resolve macro just to be clean
#undef DG_STORAGE_RESOLVE
