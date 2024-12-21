/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * Storage facilities
 */

#pragma once

#include <inttypes.h>
#include <stdlib.h>

#include "error.h"
#include "stream.h"

// String type for the path
typedef const char *DgStoragePath;

// The FILE*-like object
typedef struct DgStream DgStream;

// Type of storage object
typedef enum DgStorageObjectType {
	DG_STORAGE_TYPE_NONE = 0,            // File does not exist
	DG_STORAGE_TYPE_FILE = (1 << 0),     // File is a normal file
	DG_STORAGE_TYPE_FOLDER = (1 << 1),   // File is a folder
	DG_STORAGE_TYPE_LINK = (1 << 2),     // File is a link to a file or folder
	DG_STORAGE_TYPE_REMOTE = (1 << 30),  // Will not specify type because it is remote
	DG_STORAGE_TYPE_OTHER = (1 << 31),   // It is a different type (e.g. device on UNIX-like system)
} DgStorageObjectType;

/** Function pointers for various storage and stream operations */
/** @note Please update the wiki if any of these things change! */
typedef struct DgStorage DgStorage;
typedef struct DgStoragePool DgStoragePool;

// Filesystems
typedef DgError (*DgStorageDeleteFunction)(DgStorage *storage, DgStoragePool *pool, DgStoragePath path);
typedef DgError (*DgStorageRenameFunction)(DgStorage *storage, DgStoragePool *pool, DgStoragePath old_path, DgStoragePath new_path);
typedef DgError (*DgStorageCreateFileFunction)(DgStorage *storage, DgStoragePool *pool, DgStoragePath path);
typedef DgError (*DgStorageCreateFolderFunction)(DgStorage *storage, DgStoragePool *pool, DgStoragePath path);
typedef DgError (*DgStorageTypeFunction)(DgStorage *storage, DgStoragePool *pool, DgStoragePath path, DgStorageObjectType *type);

// Specific config destruction
typedef DgError (*DgStorageFreePoolFunction)(DgStoragePool *pool);

/**
 * The storage function array/pointer.
 */
typedef struct DgStorageFunctions {
	// Storage functions
	DgStorageCreateFileFunction create_file;
	DgStorageCreateFolderFunction create_folder;
	DgStorageTypeFunction type;
	DgStorageRenameFunction rename;
	DgStorageDeleteFunction delete;
	
	// Specific config destruction
	DgStorageFreePoolFunction free_pool;
} DgStorageFunctions;

/**
 * Configuration for a single pool, which can be an instance of one of many pool
 * classes
 */
typedef struct DgStoragePool {
	// example: = "fs", "info", "ramdisk0", "assets", "http" etc
	// NEW: may also be NULL
	DgStoragePath protocol;
	
	// Function pointer array
	// This is meant to point to static memory that has the functions for a pool class
	DgStorageFunctions *functions;
	
	// Stream implementation functions
	DgStreamImp *stream_imp;
	
	// This is the specific config for this protocol. It is passed to the
	// functions and can contain any relevant info for that type of storage.
	void *specific_config;
} DgStoragePool;

/**
 * Storage stream info
 */
typedef struct DgStorageStream {
	DgStorage *storage;
	DgStoragePool *pool;
	void *context;
} DgStorageStream;

/**
 * Storage stream path info
 */
typedef struct DgStorageStreamPath {
	DgStorage *storage;
	DgStoragePool *pool;
	DgStoragePath path;
} DgStorageStreamPath;

/**
 * Configuration for storage, which can contain many pools (protocols)
 */
typedef struct DgStorage {
	DgStoragePool *pool;
	size_t         pool_count;
} DgStorage;

DgError DgStorageInit(DgStorage *this);
DgError DgStorageFree(DgStorage *this);

// Pool manegement
DgError DgStorageAddPool(DgStorage *this, DgStoragePool *pool);
DgError DgStorageRemovePool(DgStorage *this, DgStoragePath protocol); // Automatically calls free!
DgError DgStorageGetPool(DgStorage *this, DgStoragePath protocol, DgStoragePool **pool);
bool DgStorageHasPool(DgStorage *this, DgStoragePath protocol);
DgError DgStorageRenamePool(DgStorage *this, DgStoragePath old_protocol, DgStoragePath new_protocol);

// Utility functions
DgError DgStorageSplitPathIntoParts(DgStoragePath path, char **protocol, char **filename);

// Standard filesystem functions
DgError DgStorageDelete(DgStorage *this, DgStoragePath path);
DgError DgStorageRename(DgStorage *this, DgStoragePath old_path, DgStoragePath new_path);
DgError DgStorageCreateFile(DgStorage *this, DgStoragePath path);
DgError DgStorageCreateFolder(DgStorage *this, DgStoragePath path);
DgError DgStorageType(DgStorage *this, DgStoragePath path, DgStorageObjectType *type);

// Generic pool free function
DgError DgStoragePoolFree(DgStoragePool *pool);

DgError DgStorageOpen(DgStorage *this, DgStream *context, DgStoragePath path, DgStreamOpenFlags flags);

DgError DgStorageLoad(DgStorage *storage, DgStoragePath path, size_t *size, void **buffer);
DgError DgStorageSave(DgStorage *storage, DgStoragePath path, size_t size, void *buffer);
DgError DgStorageAppend(DgStorage *storage, DgStoragePath path, size_t size, void *buffer);

