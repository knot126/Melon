/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * Abstract streams
 */

#include "memory.h"
#include "string.h"

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
	 * Get the position in the file stream.
	 * 
	 * @param context Stream object
	 * @param position The position to go to
	 * @return Error code
	 */
	
	return context->imp->set_position ? context->imp->set_position(context, position) : DG_ERROR_NOT_SUPPORTED;
}

DgError DgStreamSeek(DgStream *context, DgStreamSeekBase base, int64_t offset) {
	/**
	 * Get the position in the file stream.
	 * 
	 * @param context Stream object
	 * @param base Seek base (current pos, start, end)
	 * @param offset The offset from the base
	 * @return Error code
	 */
	
	return context->imp->seek ? context->imp->seek(context, base, offset) : DG_ERROR_NOT_SUPPORTED;
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

// Some extra storage functions that are automatically generated and for which
// there are too many of to put in this file directly.
#include "storage_generated.c.part"

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
