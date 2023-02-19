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

#pragma once

#include "error.h"

// String type for the path
typedef const char *DgStoragePath;

// The FILE*-like object
typedef void *DgStorageStreamContext;

// Type of storage object
typedef enum DgStorageObjectType {
	DG_STORAGE_TYPE_NONE = 0,            // File does not exist
	DG_STORAGE_TYPE_FILE = (1 << 0),     // File is a normal file
	DG_STORAGE_TYPE_FOLDER = (1 << 1),   // File is a folder
	DG_STORAGE_TYPE_LINK = (1 << 2),     // File is a link to a file or folder
	DG_STORAGE_TYPE_REMOTE = (1 << 30),  // Will not specify type because it is remote
	DG_STORAGE_TYPE_OTHER = (1 << 31),   // It is a different type (e.g. device on UNIX-like system)
} DgStorageObjectType;

// Storage seek base
typedef enum DgStorageSeekBase {
	DG_STORAGE_SEEK_RELATIVE = 0,
	DG_STORAGE_SEEK_START = 1,
	DG_STORAGE_SEEK_END = 2,
} DgStorageSeekBase;

// File open flags
typedef enum DgStorageFlags {
	DG_STREAM_READ = (1 << 0),
	DG_STREAM_WRITE = (1 << 1),
	DG_STREAM_APPEND = (1 << 2),
} DgStorageFlags;

/** Function pointers for various storage and stream operations */
/** @note Please update the wiki if any of these things change! */
typedef struct DgStorage DgStorage;
typedef struct DgStoragePool DgStoragePool;

// Streams (only primitive operations on which others can be built)
typedef DgError (*DgStorageOpenFunction)(DgStorage *storage, DgStoragePool *pool, DgStorageStreamContext *context, DgStoragePath path, DgStorageFlags flags); // Path has protocol removed
typedef DgError (*DgStorageCloseFunction)(DgStorage *storage, DgStoragePool *pool, DgStorageStreamContext context);

typedef DgError (*DgStorageReadFunction)(DgStorage *storage, DgStoragePool *pool, DgStorageStreamContext context, size_t size, void *buffer);
typedef DgError (*DgStorageWriteFunction)(DgStorage *storage, DgStoragePool *pool, DgStorageStreamContext context, size_t size, void *buffer);

typedef DgError (*DgStorageGetPositionFunction)(DgStorage *storage, DgStoragePool *pool, DgStorageStreamContext context, size_t *position);
typedef DgError (*DgStorageSetPositionFunction)(DgStorage *storage, DgStoragePool *pool, DgStorageStreamContext context, size_t position);
typedef DgError (*DgStorageSeekFunction)(DgStorage *storage, DgStoragePool *pool, DgStorageStreamContext context, DgStorageSeekBase base, size_t offset);

// Filesystems
typedef DgError (*DgStorageDeleteFunction)(DgStorage *storage, DgStoragePool *pool, DgStoragePath path);
typedef DgError (*DgStorageRenameFunction)(DgStorage *storage, DgStoragePool *pool, DgStoragePath old_path, DgStoragePath new_path);
typedef DgError (*DgStorageCreateFileFunction)(DgStorage *storage, DgStoragePool *pool, DgStoragePath path);
typedef DgError (*DgStorageCreateFolderFunction)(DgStorage *storage, DgStoragePool *pool, DgStoragePath path);
typedef DgError (*DgStorageTypeFunction)(DgStorage *storage, DgStoragePool *pool, DgStoragePath path, DgStorageObjectType *type);

// Specific config destruction
typedef DgError (*DgStorageFreeSpecificConfig)(DgStoragePool *pool);

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
	
	/// Stream functions (@todo rename to DgStream)
	DgStorageOpenFunction open;
	DgStorageCloseFunction close;
	DgStorageReadFunction read;
	DgStorageWriteFunction write;
	DgStorageGetPositionFunction get_position;
	DgStorageSetPositionFunction set_position;
	DgStorageSeekFunction seek;
	
	// Specific config destruction
	DgStorageFreeSpecificConfig free_specific_config;
} DgStorageFunctions;

/**
 * Configuration for a single pool, which can be an instance of one of many pool
 * classes
 */
typedef struct DgStoragePool {
	DgStoragePath protocol; // example: = "fs", "info", "ramdisk0", "assets", "http" etc
	
	void *specific_config; // This is the specific config for this protocol. It
	                       // is passed to the functions and can contain any
	                       // relevant info for that type of storage.
	
	// Function pointer array
	// This is meant to point to static memory that has the functions for a pool class
	DgStorageFunctions *functions;
} DgStoragePool;

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

// Standard filesystem functions
DgError DgStorageDelete(DgStorage *this, DgStoragePath path);
DgError DgStorageRename(DgStorage *this, DgStoragePath old_path, DgStoragePath new_path);
DgError DgStorageCreateFile(DgStorage *this, DgStoragePath path);
DgError DgStorageCreateFolder(DgStorage *this, DgStoragePath path);
DgStorageObjectType DgStorageType(DgStorage *this, DgStoragePath path);

// Generic pool free function
DgError DgStoragePoolFree(DgStoragePool *pool);

/* Abstract stream API */
#if 0
typedef struct DgStream {
    DgStoragePool *pool;
    DgStorageStreamContext context;
} DgStream;
#endif

