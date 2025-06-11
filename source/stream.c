/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Abstract streams
 */

#include "memory.h"
#include "string.h"
#include "string_builder.h"

#include "stream.h"

DgError DgStreamOpen(DgStream *context, DgStreamImp *imp, const void *path, DgStreamOpenFlags flags) {
	/**
	 * Open a stream given the imp, path and flags. This isn't meant to be
	 * called by users but instead by a function wrapping around the open which
	 * sets imp to whatever it likes and has a proper type for the path.
	 * 
	 * @param context Uninitialised stream object
	 * @param imp Stream read/write/etc implementation structure
	 * @param path Path of the stream
	 * @param flags Flags used when opening the stream
	 */
	
	// Zero it
	DgMemoryZero(context, sizeof *context);
	
	// Set imp
	context->imp = imp;
	
	// Set endianess
	if (flags & DG_STREAM_ENDIAN_BIG) {
		DgStreamSetEndian(context, DG_ENDIAN_BIG);
	}
	else if (flags & DG_STREAM_ENDIAN_NATIVE) {
		// nop
	}
	else {
		// Little (the default)
		DgStreamSetEndian(context, DG_ENDIAN_LITTLE);
	}
	
	// Call the stream's open function
	DgError status = context->imp->open ? context->imp->open(context, path, flags) : DG_ERROR_NOT_SUPPORTED;
	
	// Seek to end if the user wants that
	if (flags & DG_STREAM_START_AT_END) {
		DgStreamSeek(context, DG_STREAM_SEEK_END, 0);
	}
	
	return status;
}

DgError DgStreamClose(DgStream *context) {
	/**
	 * Close the file stream
	 * 
	 * @param context Stream object
	 * @return Error code
	 */
	
	return context->imp->close ? context->imp->close(context) : DG_ERROR_NOT_SUPPORTED;
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
	
	return context->imp->read ? context->imp->read(context, size, buffer) : DG_ERROR_NOT_SUPPORTED;
}

DgError DgStreamWrite(DgStream *context, size_t size, const void *buffer) {
	/**
	 * Write a buffer to the file stream
	 * 
	 * @param context Stream object
	 * @param size Size of the buffer
	 * @param buffer Pointer to where to store the data
	 * @return Error code
	 */
	
	return context->imp->write ? context->imp->write(context, size, buffer) : DG_ERROR_NOT_SUPPORTED;
}

DgError DgStreamGetPosition(DgStream *context, size_t *position) {
	/**
	 * Get the position in the file stream.
	 * 
	 * @param context Stream object
	 * @param position Where to put the position
	 * @return Error code
	 */
	
	return context->imp->get_position ? context->imp->get_position(context, position) : DG_ERROR_NOT_SUPPORTED;
}

DgError DgStreamSetPosition(DgStream *context, size_t position) {
	/**
	 * Set the position in the file stream.
	 * 
	 * @param context Stream object
	 * @param position The position to go to
	 * @return Error code
	 */
	
	return DgStreamSeek(context, DG_STREAM_SEEK_START, position);
}

DgError DgStreamSeek(DgStream *context, DgStreamSeekBase base, int64_t offset) {
	/**
	 * Seek to the position in the file stream.
	 * 
	 * @param context Stream object
	 * @param base Seek base (current pos, start, end)
	 * @param offset The offset from the base
	 * @return Error code
	 */
	
	return context->imp->seek ? context->imp->seek(context, base, offset) : DG_ERROR_NOT_SUPPORTED;
}

bool DgStreamSkip(DgStream *context, int64_t offset) {
	/**
	 * Skip a specific number of bytes of the file stream
	 * 
	 * @param context Stream
	 * @param offset Number of bytes to skip
	 * @return Success status
	 */
	
	return DgStreamSeek(context, DG_STREAM_SEEK_RELATIVE, offset) == DG_SUCCESS;
}

bool DgStreamEOF(DgStream *context) {
	/**
	 * Return true if the stream is at the end of file, and false if it's not.
	 * 
	 * @param context Stream object
	 * @return if the stream is at eof
	 */
	
	return context->imp->eof ? context->imp->eof(context) : false;
}

void DgStreamSetEndian(DgStream *context, bool endianness) {
	/**
	 * Set the endianness of the given stream.
	 * 
	 * @note Setting endianness only controls integers and floats using the read
	 * and write functions.
	 * 
	 * @param context Context to set endianness of
	 * @param endianness What to set the endianess to. Either DG_ENDIAN_BIG or
	 * DG_ENDIAN_LITTLE.
	 */
	
	// I dobut ?: is needed
	context->swap_endian = (endianness != DgMachineEndian()) ? true : false;
}

bool DgStreamGetEndian(DgStream *context) {
	/**
	 * Get the current endianness of the stream.
	 * 
	 * @param context Stream to get the endianness of
	 * @return DG_ENDIAN_LITTLE or DG_ENDIAN_BIG
	 */
	
	bool me = DgMachineEndian();
	
	if (me == DG_ENDIAN_LITTLE) {
		return (context->swap_endian) ? DG_ENDIAN_BIG : DG_ENDIAN_LITTLE;
	}
	else {
		return (context->swap_endian) ? DG_ENDIAN_LITTLE : DG_ENDIAN_BIG;
	}
}

bool DgStreamIsSwappingEndian(DgStream *context) {
	/**
	 * Return if the endian is being swapped on the stream.
	 * 
	 * @param context Context to check
	 * @return true if swapping endian, false if not
	 */
	
	return context->swap_endian;
}

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
	
	status = DgStreamSeek(context, DG_STREAM_SEEK_END, 0);
	
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

DgError DgStreamLoad(DgStream *context, size_t *size, void **buffer, bool add_nul) {
	/**
	 * Load the entire contents of a stream supporting seeking to start and end.
	 * 
	 * @warning You must free the buffer yourself
	 * 
	 * @param context Stream object to load from
	 * @param size Where size is written (may be NULL)
	 * @param buffer Pointer to the stream contents
	 * @param add_nul If true, an extra NUL byte is appended to the buffer
	 */
	
	size_t old_pos, length;
	
	// Store old pos for restoring later
	DgError error = DgStreamGetPosition(context, &old_pos);
	
	if (error) {
		return error;
	}
	
	// Seek to end for getting length
	error = DgStreamSeek(context, DG_STREAM_SEEK_END, 0);
	
	if (error) {
		return error;
	}
	
	// Get length
	error = DgStreamGetPosition(context, &length);
	
	if (error) {
		return error;
	}
	
	// Seek to start for reading
	error = DgStreamSeek(context, DG_STREAM_SEEK_START, 0);
	
	if (error) {
		return error;
	}
	
	// Allocate memory for buffer
	void *buf = DgMemoryAllocate(length + add_nul);
	
	if (!buf) {
		return DG_ERROR_ALLOCATION_FAILED;
	}
	
	// Read stream contents into buffer
	error = DgStreamRead(context, length, buf);
	
	if (error) {
		DgMemoryFree(buf);
		return error;
	}
	
	// Set extra NUL byte, if needed
	if (add_nul) {
		((uint8_t *) buf)[length] = '\0';
	}
	
	// Seek back to original position
	// Errors are ignored because it's kind of a waste to fail after
	// successfully reading the buffer.
	DgStreamSetPosition(context, old_pos);
	
	// Set size and buf, return success
	if (size) {
		*size = length;
	}
	
	*buffer = buf;
	return DG_SUCCESS;
}

// Some extra storage functions that are automatically generated and for which
// there are too many of to put in this file directly.
#include "stream_generated.c.part"

char *DgStreamReadStringOfSize(DgStream * restrict context, size_t size) {
	/**
	 * Read a string of a given length into a dynamic buffer and return it.
	 * 
	 * @warning You must free the returned string.
	 * 
	 * @param context Stream object
	 * @param size Size of the string in bytes
	 * @return Pointer to a NUL-terminated string, or NULL on failure
	 */
	
	char *string = DgMemoryAllocate(size + 1);
	
	if (!string) {
		return NULL;
	}
	
	if (DgStreamRead(context, size, string)) {
		DgMemoryFree(string);
		return NULL;
	}
	
	string[size] = '\0';
	
	return string;
}

char *DgStreamReadShortLine(DgStream * restrict context) {
	DgStringBuilder sb;
	DgStringBuilderClear(&sb);
	
	char ch = 0;
	
	while (ch != '\n' && DgStreamRead(context, 1, &ch) == DG_ERROR_SUCCESS) {
		if (ch != '\r') {
			DgStringBuilderAppendChar(&sb, ch);
		}
	}
	
	return DgStringBuilderGet(&sb);
}

DgError DgStreamWriteString(DgStream * restrict context, const char * restrict data) {
	/**
	 * Write a string to a stream.
	 * 
	 * @param context Stream object
	 * @param data The string to write
	 * @return Error code
	 */
	
	return DgStreamWrite(context, DgStringLength(data), (void *) data);
}

DgError DgStreamWriteIntegerString(DgStream *context, int64_t data) {
	/**
	 * Write a stringified integer to a stream WITHOUT a null byte.
	 * 
	 * @param context Stream object
	 * @param data The string to write
	 * @return Error code
	 */
	
	char *str = DgIntegerToString(10, data);
	
	if (!str) {
		return DG_ERROR_ALLOCATION_FAILED;
	}
	
	DgError error = DgStreamWrite(context, DgStringLength(str), (void *) str);
	
	DgMemoryFree(str);
	
	return error;
}

DgError DgStreamReadLEB128(DgStream *this, uint64_t *value) {
	/**
	 * Read an LEB128 encoded unsigned integer.
	 * 
	 * @param this Stream to read from
	 * @param value Pointer to where the value will be stored
	 * @return Any error while reading the integer; the stream may not be in a
	 * consistent state.
	 */
	
	DgError error = DG_ERROR_SUCCESS;
	uint64_t result = 0;
	bool reading = true;
	
	for (size_t i = 0; reading; i++) {
		uint8_t part = DgStreamReadUInt8(this, &error);
		if (error) { break; }
		result |= part << (7 * i);
		reading = (part >> 7);
	}
	
	if (!error) {
		*value = result;
	}
	
	return error;
}

DgError DgStreamWriteLEB128(DgStream *this, uint64_t value) {
	/**
	 * Write a LEB128 encoded unsigned integer.
	 * 
	 * @param this Stream to write to
	 * @param value Value to write
	 * @return Error while writing the integer; the integer could be only
	 * partially written
	 */
	
	DgError error;
	
	while (value) {
		uint8_t part = value & 0x7f;
		value >>= 7;
		part |= (value ? 0x80 : 0);
		error = DgStreamWriteUInt8(this, part);
		if (error) { break; }
	}
	
	return error;
}
