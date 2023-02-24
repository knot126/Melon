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
 * File system storage class
 */

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>

#include "alloc.h"
#include "string.h"
#include "storage.h"
#include "log.h"

#include "storage_filesystem.h"

typedef struct {
	const char *basedir;
} DgFilesytem_SpecificConfig;

static const char *DgFilesystemRealPath(const char *basedir, DgStoragePath abstract) {
	/**
	 * Find the real path of `abstract` using the base folder `basedir` and
	 * return the result.
	 * 
	 * @warning You need to free the string returned by this function!
	 * 
	 * @warning There are currently no security checks to make sure we don't
	 * span outside of the given folder! This should be fixed! (FIXME)
	 * 
	 * @param basedir The base directory the path comes from
	 * @param abstract The virtual/abstract path name
	 * @return Real path or NULL on failure
	 */
	
	char *path;
	
	if (DgStorageSplitPathIntoParts(abstract, NULL, &path)) {
		return NULL;
	}
	
	char *result = DgStringConcatinate(basedir, path);
	
	DgFree((void *) path);
	
	return result;
}

DgError DgFilesystemMakedirs(const char *deepest, bool last) {
	/**
	 * Make the directory up to `deepest`.
	 * 
	 * @param deepest The deepest dir
	 * @return Error code
	 */
	
	char *path = DgStringDuplicate(deepest);
	size_t length = DgStringLength(path);
	int status;
	
	// Create every directory before the top file/dir
	for (size_t i = 0; i < length; i++) {
		if (path[i] == '/'
		#ifdef _WIN32
			|| path[i] == '\\'
		#endif
		) {
			// Set the string up to here
			path[i] = '\0';
			
			// Make the directory
			status = mkdir(path);
			
			// Set the string back once again
			path[i] = '/';
		}
	}
	
	// If we should also create the last directory
	if (last) {
		status = mkdir(path);
	}
	
	DgFree((void *) path);
	
	return status ? DG_ERROR_FAILED : DG_ERROR_SUCCESSFUL;
}

#define REAL_PATH(VAR) DgFilesystemRealPath(((DgFilesytem_SpecificConfig *) pool->specific_config)->basedir, VAR);

static DgError DgFilesystem_Rename(DgStorage *storage, DgStoragePool *pool, DgStoragePath old_path, DgStoragePath new_path) {
	/**
	 * Rename a file in this pool
	 * 
	 * @param storage Storage object
	 * @param pool Pool that the file belongs to
	 * @param old_path Old file name
	 * @param new_path New file name
	 * @return Error code
	 */
	
	old_path = REAL_PATH(old_path);
	new_path = REAL_PATH(new_path);
	
	rename(old_path, new_path);
	
	DgFree((void *) old_path);
	DgFree((void *) new_path);
	
	return DG_ERROR_SUCCESSFUL;
}

static DgError DgFilesystem_Delete(DgStorage *storage, DgStoragePool *pool, DgStoragePath path) {
	/**
	 * Remove a file in this pool
	 * 
	 * @param storage Storage object
	 * @param pool Pool that the file belongs to
	 * @param path File name
	 * @return Error code
	 */
	
	path = REAL_PATH(path);
	
	remove(path);
	
	DgFree((void *) path);
	
	return DG_ERROR_SUCCESSFUL;
}

static DgError DgFilesystem_CreateFile(DgStorage *storage, DgStoragePool *pool, DgStoragePath path) {
	/**
	 * Create an empty file in this pool. This is similar to the posix touch command.
	 * 
	 * @param storage Storage object
	 * @param pool Pool that the file belongs to
	 * @param path File name
	 * @return Error code
	 */
	
	path = REAL_PATH(path);
	
	DgFilesystemMakedirs(path, false);
	
	// Touch the file
	FILE *f = fopen(path, "w");
	
	DgFree((void *) path);
	
	if (f) {
		fclose(f);
	}
	else {
		return DG_ERROR_FAILED;
	}
	
	return DG_ERROR_SUCCESSFUL;
}

static DgError DgFilesystem_CreateFolder(DgStorage *storage, DgStoragePool *pool, DgStoragePath path) {
	/**
	 * Create the specified folder(s)
	 * 
	 * @param storage Storage object
	 * @param pool Pool that the file belongs to
	 * @param path Folder name
	 * @return Error code
	 */
	
	path = REAL_PATH(path);
	
	DgError status = DgFilesystemMakedirs(path, true);
	
	DgFree((void *) path);
	
	return status;
}

static DgError DgFilesystem_Type(DgStorage *storage, DgStoragePool *pool, DgStoragePath path, DgStorageObjectType *type) {
	/**
	 * Find the type of file at path
	 * 
	 * @param storage Storage object
	 * @param pool Pool that the file belongs to
	 * @param path File name
	 * @param type Where to put the result
	 * @return Error code
	 */
	
	DgLog(DG_LOG_ERROR, "DgFilesystem_Type()", path);
	
	return DG_ERROR_NOT_IMPLEMENTED;
}

static DgError DgFilesystem_Open(DgStorage *storage, DgStoragePool *pool, DgStream *context, DgStoragePath path, DgStorageFlags flags) {
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
	
	path = REAL_PATH(path);
	
	char *mode = "rb";
	
	// Find the mode that matches the given flags the best.
	if ((flags & DG_STREAM_READ) && !(flags & DG_STREAM_WRITE)) {
		mode = "rb";
	}
	else if (!(flags & DG_STREAM_READ) && (flags & DG_STREAM_WRITE)) {
		mode = "wb";
	}
	else if ((flags & DG_STREAM_READ) && (flags & DG_STREAM_WRITE)) {
		mode = "r+b";
		
		// Make sure the file exists first!
		// This will create the file if it doesn't exist but won't do anything
		// if it does.
		FILE *temp = fopen(path, "w+x");
		
		if (temp) {
			fclose(temp);
		}
	}
	else {
		DgFree((void *) path);
		return DG_ERROR_NOT_SUPPORTED;
	}
	
	DgLog(DG_LOG_VERBOSE, "Opening file %s in mode %s", path, mode);
	
	context->context = (void *) fopen(path, mode);
	
	DgFree((void *) path);
	
	if (!context->context) {
		return DG_ERROR_FAILED;
	}
	
	// Seek to the end if the user wants that
	if (flags & DG_STREAM_START_AT_END) {
		fseek((FILE *) context->context, 0, SEEK_END);
	}
	
	return DG_ERROR_SUCCESSFUL;
}

static DgError DgFilesystem_Close(DgStorage *storage, DgStoragePool *pool, DgStream *context) {
	/**
	 * Close a file stream
	 * 
	 * @param storage Storage object
	 * @param pool Pool that the file belongs to
	 * @param stream Stream object
	 * @return Error code
	 */
	
	fclose((FILE *) context->context);
	
	return DG_ERROR_SUCCESSFUL;
}

static DgError DgFilesystem_Read(DgStorage *storage, DgStoragePool *pool, DgStream *context, size_t size, void *buffer) {
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
	
	size_t result = fread(buffer, 1, size, (FILE *) context->context);
	
	if (result != size) {
		return DG_ERROR_FAILED;
	}
	
	return DG_ERROR_SUCCESSFUL;
}

static DgError DgFilesystem_Write(DgStorage *storage, DgStoragePool *pool, DgStream *context, size_t size, void *buffer) {
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
	
	size_t result = fwrite(buffer, 1, size, (FILE *) context->context);
	
	if (result != size) {
		return DG_ERROR_FAILED;
	}
	
	return DG_ERROR_SUCCESSFUL;
}

static DgError DgFilesystem_GetPosition(DgStorage *storage, DgStoragePool *pool, DgStream *context, size_t *position) {
	/**
	 * Get the current position of a file stream
	 * 
	 * @param storage Storage object
	 * @param pool Pool that the file belongs to
	 * @param context Stream object
	 * @param position Where to put the positon
	 * @return Error code
	 */
	
	position[0] = ftell((FILE *) context->context);
	
	return DG_ERROR_SUCCESSFUL;
}

static DgError DgFilesystem_SetPosition(DgStorage *storage, DgStoragePool *pool, DgStream *context, size_t position) {
	/**
	 * Get the current position of a file stream
	 * 
	 * @param storage Storage object
	 * @param pool Pool that the file belongs to
	 * @param context Stream object
	 * @param position What to set the position to
	 * @return Error code
	 */
	
	int status = fseek((FILE *) context->context, position, SEEK_SET);
	
	if (status) {
		return DG_ERROR_FAILED;
	}
	
	return DG_ERROR_SUCCESSFUL;
}

static DgError DgFilesystem_Seek(DgStorage *storage, DgStoragePool *pool, DgStream *context, DgStorageSeekBase base, int64_t offset) {
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
	
	int origin;
	
	switch (base) {
		case DG_STORAGE_SEEK_RELATIVE: origin = SEEK_CUR; break;
		case DG_STORAGE_SEEK_START:    origin = SEEK_SET; break;
		case DG_STORAGE_SEEK_END:      origin = SEEK_END; break;
	}
	
	int status = fseek((FILE *) context->context, offset, origin);
	
	if (status) {
		return DG_ERROR_FAILED;
	}
	
	return DG_ERROR_SUCCESSFUL;
}

static DgError DgFilesystem_FreeSpecificConfig(DgStoragePool *pool) {
	DgFree((void *) ((DgFilesytem_SpecificConfig *) pool->specific_config)->basedir);
	DgFree(pool->specific_config);
	
	return DG_ERROR_SUCCESSFUL;
}

#undef REAL_PATH

DgStorageFunctions gStorageFilesystemFunctions = {
	.create_file = &DgFilesystem_CreateFile,
	.create_folder = &DgFilesystem_CreateFolder,
	.type = &DgFilesystem_Type,
	.rename = &DgFilesystem_Rename,
	.delete = &DgFilesystem_Delete,
	.open = &DgFilesystem_Open,
	.close = &DgFilesystem_Close,
	.read = &DgFilesystem_Read,
	.write = &DgFilesystem_Write,
	.get_position = &DgFilesystem_GetPosition,
	.set_position = &DgFilesystem_SetPosition,
	.seek = &DgFilesystem_Seek,
	.free_specific_config = &DgFilesystem_FreeSpecificConfig,
};

DgStoragePool *DgFilesystemCreatePool(const char *protocol, const char *basedir) {
	/**
	 * Create a filesystem-based pool
	 * 
	 * @param protocol Protocol string this pool can be accessed by
	 * @param basedir The base directory
	 * @return Pointer to the storage pool (or NULL if failed)
	 */
	
	DgStoragePool *pool = DgAlloc(sizeof *pool);
	
	if (!pool) {
		return NULL;
	}
	
	pool->protocol = DgStringDuplicate(protocol);
	pool->functions = &gStorageFilesystemFunctions;
	pool->specific_config = DgAlloc(sizeof *pool->specific_config);
	
	if (!pool->specific_config) {
		DgFree(pool);
		return NULL;
	}
	
	DgFilesytem_SpecificConfig *config = (DgFilesytem_SpecificConfig *) pool->specific_config;
	
	// We also pre-append the / if there isn't one already
	config->basedir = DgStringConcatinate(basedir, "/");
	
	return pool;
}
