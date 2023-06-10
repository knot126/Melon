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

#include <inttypes.h>

#include "error.h"

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
	DG_STREAM_START_AT_END = (1 << 2),
	DG_STREAM_DONT_OVERWRITE = (1 << 3),
} DgStorageFlags;

/** Function pointers for various storage and stream operations */
/** @note Please update the wiki if any of these things change! */
typedef struct DgStorage DgStorage;
typedef struct DgStoragePool DgStoragePool;

// Streams (only primitive operations on which others can be built)
typedef DgError (*DgStorageOpenFunction)(DgStorage *storage, DgStoragePool *pool, DgStream *context, DgStoragePath path, DgStorageFlags flags);
typedef DgError (*DgStorageCloseFunction)(DgStorage *storage, DgStoragePool *pool, DgStream *context);

typedef DgError (*DgStorageReadFunction)(DgStorage *storage, DgStoragePool *pool, DgStream *context, size_t size, void *buffer);
typedef DgError (*DgStorageWriteFunction)(DgStorage *storage, DgStoragePool *pool, DgStream *context, size_t size, void *buffer);

typedef DgError (*DgStorageGetPositionFunction)(DgStorage *storage, DgStoragePool *pool, DgStream *context, size_t *position);
typedef DgError (*DgStorageSetPositionFunction)(DgStorage *storage, DgStoragePool *pool, DgStream *context, size_t position);
typedef DgError (*DgStorageSeekFunction)(DgStorage *storage, DgStoragePool *pool, DgStream *context, DgStorageSeekBase base, int64_t offset);

// Filesystems
typedef DgError (*DgStorageDeleteFunction)(DgStorage *storage, DgStoragePool *pool, DgStoragePath path);
typedef DgError (*DgStorageRenameFunction)(DgStorage *storage, DgStoragePool *pool, DgStoragePath old_path, DgStoragePath new_path);
typedef DgError (*DgStorageCreateFileFunction)(DgStorage *storage, DgStoragePool *pool, DgStoragePath path);
typedef DgError (*DgStorageCreateFolderFunction)(DgStorage *storage, DgStoragePool *pool, DgStoragePath path);
typedef DgError (*DgStorageTypeFunction)(DgStorage *storage, DgStoragePool *pool, DgStoragePath path, DgStorageObjectType *type);

// Specific config destruction
typedef DgError (*DgStorageFreeSpecificConfigFunction)(DgStoragePool *pool);

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
	DgStorageFreeSpecificConfigFunction free_specific_config;
} DgStorageFunctions;

/**
 * Configuration for a single pool, which can be an instance of one of many pool
 * classes
 */
typedef struct DgStoragePool {
	// example: = "fs", "info", "ramdisk0", "assets", "http" etc
	DgStoragePath protocol;
	
	// Function pointer array
	// This is meant to point to static memory that has the functions for a pool class
	DgStorageFunctions *functions;
	
	// This is the specific config for this protocol. It is passed to the
	// functions and can contain any relevant info for that type of storage.
	void *specific_config;
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

// Utility functions
DgError DgStorageSplitPathIntoParts(DgStoragePath path, char **protocol, char **filename);

// Standard filesystem functions
DgError DgStorageDelete(DgStorage *this, DgStoragePath path);
DgError DgStorageRename(DgStorage *this, DgStoragePath old_path, DgStoragePath new_path);
DgError DgStorageCreateFile(DgStorage *this, DgStoragePath path);
DgError DgStorageCreateFolder(DgStorage *this, DgStoragePath path);
DgStorageObjectType DgStorageType(DgStorage *this, DgStoragePath path, DgStorageObjectType *type);

// Generic pool free function
DgError DgStoragePoolFree(DgStoragePool *pool);

/* Abstract stream API */
typedef struct DgStream {
	DgStorage *storage;
	DgStoragePool *pool;
	void *context;
} DgStream;

DgError DgStreamOpen(DgStorage *this, DgStream *context, DgStoragePath path, DgStorageFlags flags);
DgError DgStreamClose(DgStream *context);

DgError DgStreamRead(DgStream *context, size_t size, void *buffer);
DgError DgStreamWrite(DgStream *context, size_t size, void *buffer);

DgError DgStreamGetPosition(DgStream *context, size_t *position);
DgError DgStreamSetPosition(DgStream *context, size_t position);
DgError DgStreamSeek(DgStream *context, DgStorageSeekBase base, int64_t offset);

// Extra functions
size_t DgStreamLength(DgStream *context);

// Even more functions!
#include "storage_generated.h"
