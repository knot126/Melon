/**
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * File System Utilites
 * 
 * @todo This library is in need of a major rewrite to support the new File
 * Systems concept as well as to update the crufty parts.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#if defined(__linux__)
	#include <sys/stat.h>
	#include <dirent.h>
	#include <errno.h>
#endif

#include "alloc.h"
#include "types.h"
#include "log.h"
#include "str.h"

#include "fs.h"

const char * const ASSETS_LOOK_PATH[] = {
	"./assets",
	"../Engine/assets",
	"../Trestle/assets",
	"../Dragonfruit/assets",
	"../assets",
};

const char *dg_special_directory_paths[3] = {
	NULL,
	NULL,
	".",
};

void DgInitPaths(uint32_t fail_mode) {
	/**
	 * This will initialise the paths in dg_special_directory_paths using
	 * automatic pathfinding.
	 * 
	 * @deprecated Use file systems instead
	 * 
	 * @param fail_mode
	 */
	
	// Assets path
	for (int i = 0; i < sizeof(ASSETS_LOOK_PATH); i++) {
		bool d = DgIsDir(ASSETS_LOOK_PATH[i]);
		
		if (d) {
			dg_special_directory_paths[0] = ASSETS_LOOK_PATH[i];
			break;
		}
	}
	
	if (!dg_special_directory_paths[0]) {
		DgLog(DG_LOG_WARNING, "Failed to initialise legacy filesystem directory paths.");
	}
}

void DgInitPaths2(const char * restrict assets) {
	/**
	 * This will initialise the paths in dg_special_directory_paths using
	 * specified paths.
	 */
	
	dg_special_directory_paths[0] = assets;
}

DgFileStream* DgFileStreamOpen(char* path, char* permissions) {
	/**
	 * Opens a file stream
	 * 
	 * @param path Path to the file to open
	 * @param permissions Mode to open the file in
	 * @return File stream handle or NULL on failure
	 */
	
	FILE *f;
	
	f = fopen(path, permissions);
	
	if (!f) {
		DgLog(DG_LOG_ERROR, "Failed to open file stream at %s.", path);
		return 0;
	}
	
	DgFileStream *s = (DgFileStream *) DgAlloc(sizeof(DgFileStream));
	s->_c_file_stream = f;
	
	return s;
}

DgFile DgFileOpen2(const char * const filepath, DgFileStreamFlags flags) {
	/**
	 * Open a file stream (newer version of DgFileStreamOpen).
	 * 
	 * @param filepath Path to the file to open
	 * @param flags Flags to use when opening the file
	 * @return File stream handle, or NULL on failure
	 */
	
	DgFileStream *stream = NULL;
	char *realpath = DgEvalPath((char *) filepath);
	
	if (!realpath) {
		return NULL;
	}
	
	char *flags_string;
	
	if (flags & DG_FILE_STREAM_READ) {
		flags_string = "rb";
	}
	else if (flags & DG_FILE_STREAM_WRITE) {
		flags_string = "wb";
	}
	else {
		flags_string = "r+";
	}
	
	stream = DgFileStreamOpen(realpath, flags_string);
	
	DgFree(realpath);
	
	if (!stream) {
		return NULL;
	}
	
	return stream;
}

DgFileStream* DgFileStreamReopen(DgFileStream* stream, char* path, char* permissions) {
	return NULL;
}

DgFileStream* DgChangeStreamPermissions(DgFileStream* stream, char* permissions) {
	return NULL;
}

void DgFileStreamClose(DgFileStream* stream) {
	/**
	 * Frees a file stream
	 * 
	 * @param stream Stream to close
	 */
	
	fclose(stream->_c_file_stream);
	DgFree(stream);
}

void DgFileStreamRead(DgFileStream* stream, size_t size, void* data) {
	/**
	 * Read size bytes from stream into data
	 * 
	 * @param stream File stream handle
	 * @param size Size of data to read
	 * @param data Pointer to where to put the data
	 */
	
	fread(data, sizeof(byte), size, stream->_c_file_stream);
}

void DgFileStreamWrite(DgFileStream* stream, size_t size, const void* data) {
	/**
	 * Write size bytes from data into stream
	 * 
	 * @param stream File stream handle
	 * @param size Size of the data to write
	 * @param data The data to write to the stream
	 */
	
	fwrite(data, sizeof(byte), size, stream->_c_file_stream);
}

size_t DgFileStreamLength(DgFileStream* stream) {
	/**
	 * Get the length of the file in stream
	 * 
	 * @param stream File stream handle
	 */
	
	size_t size;
	
	// printf("<0x%X>\n", stream->_c_file_stream);
	
	fseek(stream->_c_file_stream, 0, SEEK_END);
	size = ftell(stream->_c_file_stream);
	rewind(stream->_c_file_stream);
	
	return size;
}

bool DgFileStreamEndOfFile(DgFileStream *stream) {
	/**
	 * Get if the stream has EOF marked.
	 * 
	 * @param stream File stream handle
	 */
	
	return !!feof(stream->_c_file_stream);
}

char *DgFileStreamGetString(DgFileStream* stream, size_t *length) {
	/**
	 * Get a string from a file stream. Here, a string is terminated by either 
	 * a \0, \n, EOF or \r. This returns NULL on failure or the string on
	 * success.
	 * 
	 * If length is not NULL, then the string size is written to length.
	 * 
	 * @note The returned string must be freed afterwards.
	 * 
	 * @param stream File stream handle
	 * @param length Pointer to a size_t to write the stream to, or NULL
	 * @return String read from file
	 */
	
	int c;
	size_t size = 0;
	size_t alloc = 0;
	char *str = NULL;
	
	if (DgFileStreamEndOfFile(stream)) {
		return NULL;
	}
	
	while (true) {
		if (size >= alloc) {
			alloc = 4 + (alloc * 2);
			
			str = (char *) DgRealloc(str, alloc);
			
			if (!str) {
				return NULL;
			}
		}
		
		c = fgetc(stream->_c_file_stream);
		
		if (c == '\0' || c == '\n' || c == EOF || c == '\r') {
			break;
		}
		
		str[size] = (char) c;
		
		size++;
	}
	
	str = (char *) DgRealloc(str, size + 1);
	
	if (!str) {
		return NULL;
	}
	
	str[size] = '\0';
	
	if (length) {
		*length = size;
	}
	
	return str;
}

unsigned char *DgFileStreamLoad(DgFile stream) {
	/**
	 * Load a file into memory and return the buffer that was created.
	 * 
	 * @warning The data is not gaurentted to be NULL terminated and there
	 * appears to be no way to free the data, though it can still be freed.
	 * 
	 * @param stream File stream handle
	 * @return Data loaded from file
	 */
	
	DgLog(DG_LOG_WARNING, "Using DgFileStreamLoad which does not have any protections against bad data. Please use uint8_t *DgFileLoad(path, content_size) instead !!");
	
	size_t size = DgFileStreamLength(stream);
	
	unsigned char *data = (unsigned char *) DgAlloc(size);
	
	if (!data) {
		return NULL;
	}
	
	DgFileStreamRead(stream, size, data);
	
	return data;
}

uint8_t *DgFileLoad(const char * restrict path, size_t *content_size) {
	/**
	 * Load a file into memory and return the buffer that was created.
	 * 
	 * @note If you are processing a **trusted** text file, then it is safe to
	 * provide NULL to content_size and a NUL char will be appended to the end.
	 * 
	 * @param stream File stream handle
	 * @return Data loaded from file on success, or NULL on failure
	 */
	
	// Open stream
	DgFile stream = DgFileOpen2(path, DG_FILE_STREAM_READ);
	
	if (!stream) {
		return stream;
	}
	
	// Read stream size
	size_t size = DgFileStreamLength(stream);
	
	if (content_size != NULL) {
		*content_size = size;
	}
	
	// Allocate room for data plus NUL char
	unsigned char *data = (unsigned char *) DgAlloc(size + 1);
	
	if (!data) {
		DgFileStreamClose(stream);
		return NULL;
	}
	
	// Read data in
	DgFileStreamRead(stream, size, data);
	
	// Write NUL char to end
	data[size] = '\0';
	
	// Close stream
	DgFileStreamClose(stream);
	
	return data;
}

/**
 * Non-File Stream Functions
 * -------------------------
 * 
 * Generic file utilities, not for streams!
 */

char* DgEvalPath(char* path) {
	/**
	 * Expands/evaluates a DGFS path into a real filesystem path.
	 * 
	 * @note Remember to free the allocated memory after!
	 * 
	 * @note
	 * DGFS DESCRIPTION
	 * ================
	 * 
	 * DGFS allows using a single, standard file path for all platforms. It 
	 * (tries to) abstracts the normal file system without going too far.
	 * 
	 *   * asset://            - Access to the game's main asset bundle.
	 *   * config://           - Access to the user's save directory.
	 *   * bin://              - Access to the binaries directory.
	 * 
	 * @param path Path to evaluate into a filesytem path
	 * @return String representing the real file system path
	 */
	
	// Check for a path that does not need evaluation
	if (!strstr(path, "://")) {
		return DgStrdup(path);
	}
	
	uint8_t i; // For the path to use
	
	// Note we only check the first two bytes
	if (!memcmp(path, "as", 2)) {
		i = 0;
	}
	else if (!memcmp(path, "co", 2)) {
		i = 1;
	}
	else if (!memcmp(path, "bi", 2)) {
		i = 2;
	}
	else {
		return NULL;
	}
	
	// Here, we account for: old len + added "/" len + special len + null term
	size_t size = strlen(path) + strlen(dg_special_directory_paths[i]) + (sizeof(char) * 2);
	char *new_path = (char *) DgAlloc(size);
	char *rel_path = strstr(path, "://") + (sizeof(char) * 3);
	
	strcpy(new_path, dg_special_directory_paths[i]);
	strcat(new_path, "/");
	strcat(new_path, rel_path);
	
	return new_path;
}

void DgMkdir(char* path) {
	/**
	 * Create a new directory
	 * 
	 * @param path Path to the directory to create
	 */
	
#if defined(__linux__)
	mkdir(path, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
#else
	DgLog(DG_LOG_WARNING, "Function DgMkdir() is not implemented for this platform.\n");
#endif
}

bool DgIsDir(const char* dir) {
	/**
	 * Check if a file is a directory
	 * 
	 * @param dir Path to the file to check
	 * @return If the file is a directory or not
	 */
	
	bool ret = false;
	
#if defined(__linux__)
	DIR *d = opendir(dir);
	
	if (d) {
		ret = true;
	}
	
	closedir(d);
#else
	DgLog(DG_LOG_WARNING, "Function DgIsDir is not implemented for this platform.\n");
#endif
	
	return ret;
}

char *DgGetUserDir(void) {
	/**
	 * Return the path to the user directory
	 * 
	 * @return Path to the user directory
	 */
	
	return NULL;
}

void DgDeleteFile(char* path) {
	/** 
	 * Delete a file
	 * 
	 * @deprecated Does not return error code
	 * 
	 * @param path Path to the file to remove
	 */
	
	int status = remove(path);
	
	if (status) {
		DgLog(DG_LOG_ERROR, "Failed to remove file %s.", path);
	}
}

void DgMoveFile(char* src, char* dest) {
	/** 
	 * Move a file
	 * 
	 * @deprecated Does not return error code
	 * 
	 * @param src Source file name
	 * @param dest Destination file name
	 */
	
	int status = rename(src, dest);
	
	if (status) {
		DgLog(DG_LOG_ERROR, "Failed to rename file %s.", src);
	}
}

void DgCopyFile(char* src, char* dest) {
	DgLog(DG_LOG_WARNING, "Function DgCopyFile() is not implemented.");
}

/* =============================================================================
 * Datatype reading functions 
 */

inline void DgFileStreamReadInt8(DgFileStream* stream, int8_t* data) {
	fread(data, sizeof(int8_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamReadUInt8(DgFileStream* stream, uint8_t* data) {
	fread(data, sizeof(uint8_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamReadInt16(DgFileStream* stream, int16_t* data) {
	fread(data, sizeof(int16_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamReadUInt16(DgFileStream* stream, uint16_t* data) {
	fread(data, sizeof(uint16_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamReadInt32(DgFileStream* stream, int32_t* data) {
	fread(data, sizeof(int32_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamReadUInt32(DgFileStream* stream, uint32_t* data) {
	fread(data, sizeof(uint32_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamReadInt64(DgFileStream* stream, int64_t* data) {
	fread(data, sizeof(int64_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamReadUInt64(DgFileStream* stream, uint64_t* data) {
	fread(data, sizeof(uint64_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamReadFloat(DgFileStream* stream, float* data) {
	fread(data, sizeof(float), 1, stream->_c_file_stream);
}

inline void DgFileStreamReadDouble(DgFileStream* stream, double* data) {
	fread(data, sizeof(double), 1, stream->_c_file_stream);
}

inline void DgFileStreamReadLine(DgFileStream* stream, size_t max, char *data) {
	char *c = fgets(data, max, stream->_c_file_stream);
	if (!c) {
		DgLog(DG_LOG_ERROR, "Failed to read line from file stream.");
	}
}

/* =============================================================================
 * Datatype writing functions 
 */

inline void DgFileStreamWriteInt8(DgFileStream* stream, int8_t* data) {
	fwrite(data, sizeof(int8_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamWriteUInt8(DgFileStream* stream, uint8_t* data) {
	fwrite(data, sizeof(uint8_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamWriteInt16(DgFileStream* stream, int16_t* data) {
	fwrite(data, sizeof(int16_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamWriteUInt16(DgFileStream* stream, uint16_t* data) {
	fwrite(data, sizeof(uint16_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamWriteInt32(DgFileStream* stream, int32_t* data) {
	fwrite(data, sizeof(int32_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamWriteUInt32(DgFileStream* stream, uint32_t* data) {
	fwrite(data, sizeof(uint32_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamWriteInt64(DgFileStream* stream, int64_t* data) {
	fwrite(data, sizeof(int64_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamWriteUInt64(DgFileStream* stream, uint64_t* data) {
	fwrite(data, sizeof(uint64_t), 1, stream->_c_file_stream);
}

inline void DgFileStreamWriteFloat(DgFileStream* stream, float* data) {
	fwrite(data, sizeof(float), 1, stream->_c_file_stream);
}

inline void DgFileStreamWriteDouble(DgFileStream* stream, double* data) {
	fwrite(data, sizeof(double), 1, stream->_c_file_stream);
}

inline void DgFileStreamWriteLine(DgFileStream* stream, char *data) {
	fwrite(data, strlen(data) * sizeof(char), 1, stream->_c_file_stream);
	char n = '\n';
	fwrite(&n, 1, 1, stream->_c_file_stream);
}

inline void DgFileStreamWriteString(DgFileStream* stream, char *data) {
	fwrite(data, strlen(data) * sizeof(char), 1, stream->_c_file_stream);
}

inline void DgFileStreamWriteIntegerString(DgFileStream *stream, size_t data) {
	/**
	 * Write a string as an integer.
	 */
	
	fprintf(stream->_c_file_stream, "%d", data);
}
