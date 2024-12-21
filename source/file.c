/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * Load and save file functions
 */

#include "stream.h"
#include "error.h"
#include "memory.h"
#include "log.h"

#include "file.h"

DgError DgFileStream_Open(DgStream *stream, const char *path, DgStreamOpenFlags flags) {
	const char *mode = "rb";
	
	if ((flags & DG_STREAM_READ) && (flags & DG_STREAM_WRITE)) {
		if (flags & DG_STREAM_DONT_OVERWRITE) {
			mode = "r+b";
		}
		else {
			mode = "wb";
		}
	}
	else if (flags & DG_STREAM_READ) {
		mode = "rb";
	}
	else {
		/* TODO: error, no write-only files */
	}
	
	stream->context = (void *) fopen(path, mode);
	
	if (!stream->context) {
		return DG_FAIL;
	}
	
	return DG_SUCCESS;
}

DgError DgFileStream_Close(DgStream *stream) {
	fclose((FILE *) stream->context);
	return DG_SUCCESS;
}

DgError DgFileStream_Read(DgStream *stream, size_t size, void *buffer) {
	return fread(buffer, 1, size, stream->context) == size ? DG_SUCCESS : DG_FAIL;
}

DgError DgFileStream_Write(DgStream *stream, size_t size, const void *buffer) {
	return fwrite(buffer, 1, size, stream->context) == size ? DG_SUCCESS : DG_FAIL;
}

DgError DgFileStream_GetPos(DgStream *stream, size_t *position) {
	position[0] = ftell(stream->context);
	return DG_SUCCESS;
}

DgError DgFileStream_Seek(DgStream *stream, DgStreamSeekBase base, int64_t offset) {
	int origin = SEEK_SET;
	
	switch (base) {
		case DG_STREAM_SEEK_RELATIVE: origin = SEEK_CUR; break;
		case DG_STREAM_SEEK_START: origin = SEEK_SET; break;
		case DG_STREAM_SEEK_END: origin = SEEK_END; break;
	}
	
	return fseek(stream->context, offset, origin) ? DG_FAIL : DG_SUCCESS;
}

DgStreamImp gFileStreamImp = {
	.open = (DgStreamOpenFunction) DgFileStream_Open,
	.close = DgFileStream_Close,
	.read = DgFileStream_Read,
	.write = DgFileStream_Write,
	.get_position = DgFileStream_GetPos,
	.seek = DgFileStream_Seek,
};

DgError DgFileOpen(DgStream *stream, const char *path, DgStreamOpenFlags flags) {
	/**
	 * Open a file stream backed by C standard fopen()
	 * 
	 * @param stream Stream object
	 * @param path Path of file to open
	 * @param flags File open flags
	 */
	
	return DgStreamOpen(stream, &gFileStreamImp, path, flags);
}

DgError DgFileLoad(const char *path, size_t *size, void **buffer) {
	/**
	 * Load the contents of a file into a buffer.
	 * 
	 * @warning You are responsible for freeing the buffer
	 * 
	 * @note The buffer is always appended with an extra NUL byte to ease loading
	 * files as strings
	 * 
	 * @param path Path of the file to load
	 * @param size Where the buffer size will be written (may be NULL)
	 * @param buffer Where the pointer to the buffer will be written
	 */
	
	DgStream stream;
	
	DgError error = DgFileOpen(&stream, path, DG_STREAM_READ);
	
	if (error) {
		return error;
	}
	
	error = DgStreamLoad(&stream, size, buffer, true);
	
	DgStreamClose(&stream);
	
	return error;
}
