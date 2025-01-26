/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Abstract streams
 */

#pragma once

#include "common.h"
#include "error.h"
#include "machine.h"

typedef struct DgStream DgStream;

// Storage seek base
typedef enum DgStreamSeekBase {
	DG_STREAM_SEEK_RELATIVE = 0,
	DG_STREAM_SEEK_START = 1,
	DG_STREAM_SEEK_END = 2,
} DgStreamSeekBase;

// File open flags
typedef enum DgStreamOpenFlags : uint64_t {
	DG_STREAM_READ = (1 << 0),
	DG_STREAM_WRITE = (1 << 1),
	DG_STREAM_START_AT_END = (1 << 2),
	DG_STREAM_DONT_OVERWRITE = (1 << 3),
	DG_STREAM_ENDIAN_BIG = (1 << 4),
	DG_STREAM_ENDIAN_LITTLE = (1 << 5),
	DG_STREAM_ENDIAN_NATIVE = (1 << 6),
} DgStreamOpenFlags;

// Stream function pointer types
typedef DgError (*DgStreamOpenFunction)(DgStream *context, const void *path, DgStreamOpenFlags flags);
typedef DgError (*DgStreamCloseFunction)(DgStream *context);
typedef DgError (*DgStreamReadFunction)(DgStream *context, size_t size, void *buffer);
typedef DgError (*DgStreamWriteFunction)(DgStream *context, size_t size, const void *buffer);
typedef DgError (*DgStreamGetPositionFunction)(DgStream *context, size_t *position);
typedef DgError (*DgStreamSetPositionFunction)(DgStream *context, size_t position);
typedef DgError (*DgStreamSeekFunction)(DgStream *context, DgStreamSeekBase base, int64_t offset);

typedef struct DgStreamImp {
	DgStreamOpenFunction open;
	DgStreamCloseFunction close;
	DgStreamReadFunction read;
	DgStreamWriteFunction write;
	DgStreamGetPositionFunction get_position;
	DgStreamSetPositionFunction set_position; // deprecated
	DgStreamSeekFunction seek;
} DgStreamImp;

// Generic flags that are implemented by DgStream
typedef uint64_t DgStreamFlags;

// Abstract stream structure
typedef struct DgStream {
	void *context; // The stream's specific context
	DgStreamImp *imp;
	DgStreamFlags swap_endian : 1; // If the endianness of integers and floats shoud be swapped
	DgStreamFlags _rest : 63; // Unused
} DgStream;

DgError DgStreamOpen(DgStream *context, DgStreamImp *imp, const void *path, DgStreamOpenFlags flags);
DgError DgStreamClose(DgStream *context);

DgError DgStreamRead(DgStream *context, size_t size, void *buffer);
DgError DgStreamWrite(DgStream *context, size_t size, const void *buffer);

DgError DgStreamGetPosition(DgStream *context, size_t *position);
DgError DgStreamSetPosition(DgStream *context, size_t position);
DgError DgStreamSeek(DgStream *context, DgStreamSeekBase base, int64_t offset);

void DgStreamSetEndian(DgStream *context, bool endianness);
bool DgStreamGetEndian(DgStream *context);
bool DgStreamIsSwappingEndian(DgStream *context);

size_t DgStreamLength(DgStream *context);
DgError DgStreamLoad(DgStream *context, size_t *size, void **buffer, bool add_nul);

#include "stream_generated.h.part"

DgError DgStreamWriteString(DgStream * restrict context, const char * restrict data);
DgError DgStreamWriteIntegerString(DgStream *context, int64_t data);
