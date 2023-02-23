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
 * Generic Stream Utilites
 */

#pragma once

#include <stdbool.h>
#include <inttypes.h>
#include <stdlib.h>

typedef enum DgMemoryStreamEnum {
	/* errors */
	DG_MEMORY_STREAM_OKAY = 0,
	DG_MEMORY_STREAM_ALLOC_ERROR = 1,
	DG_MEMORY_STREAM_NO_SPACE = 2,
	DG_MEMORY_STREAM_INVALID = 3,
	DG_MEMORY_STREAM_UNKNOWN = 4,
	DG_MEMORY_STREAM_EOF = 5,
	DG_MEMORY_STREAM_RANGE = 6,
	
	/* offsets */
	DG_MEMORY_STREAM_CUR = 0,
	DG_MEMORY_STREAM_SET = 1,
	DG_MEMORY_STREAM_END = 2,
} DgMemoryStreamEnum;

typedef struct DgMemoryStream {
	/* For storing data */
	uint8_t *data;
	size_t allocated;
	size_t size;
	size_t head;
	
	/* Errors */
	size_t error;
} DgMemoryStream;

DgMemoryStream *DgMemoryStreamCreate(void);
DgMemoryStream *DgMemoryStreamFromBuffer(void *buffer, size_t size);

void DgMemoryStreamFree(DgMemoryStream *stream);
void DgBufferFromStream(DgMemoryStream *stream, void **pointer, size_t *size);

size_t DgMemoryStreamError(DgMemoryStream *stream);

size_t DgMemoryStreamGetpos(DgMemoryStream *stream);
size_t DgMemoryStreamLength(DgMemoryStream *stream);
void DgMemoryStreamSetpos(DgMemoryStream *stream, DgMemoryStreamEnum offset, int64_t pos);
void DgMemoryStreamRewind(DgMemoryStream *stream);

void DgMemoryStreamRead(DgMemoryStream *stream, size_t size, void *buffer);
void DgMemoryStreamWrite(DgMemoryStream *stream, size_t size, void *buffer);

int8_t DgMemoryStreamReadInt8(DgMemoryStream *stream);
uint8_t DgMemoryStreamReadUInt8(DgMemoryStream *stream);
int16_t DgMemoryStreamReadInt16(DgMemoryStream *stream);
uint16_t DgMemoryStreamReadUInt16(DgMemoryStream *stream);
int32_t DgMemoryStreamReadInt32(DgMemoryStream *stream);
uint32_t DgMemoryStreamReadUInt32(DgMemoryStream *stream);
int64_t DgMemoryStreamReadInt64(DgMemoryStream *stream);
uint64_t DgMemoryStreamReadUInt64(DgMemoryStream *stream);
float DgMemoryStreamReadFloat(DgMemoryStream *stream);
double DgMemoryStreamReadDouble(DgMemoryStream *stream);

void DgMemoryStreamWriteInt8(DgMemoryStream *stream, int8_t *data);
void DgMemoryStreamWriteUInt8(DgMemoryStream *stream, uint8_t *data);
void DgMemoryStreamWriteInt16(DgMemoryStream *stream, int16_t *data);
void DgMemoryStreamWriteUInt16(DgMemoryStream *stream, uint16_t *data);
void DgMemoryStreamWriteInt32(DgMemoryStream *stream, int32_t *data);
void DgMemoryStreamWriteUInt32(DgMemoryStream *stream, uint32_t *data);
void DgMemoryStreamWriteInt64(DgMemoryStream *stream, int64_t *data);
void DgMemoryStreamWriteUInt64(DgMemoryStream *stream, uint64_t *data);
void DgMemoryStreamWriteFloat(DgMemoryStream *stream, float *data);
void DgMemoryStreamWriteDouble(DgMemoryStream *stream, double *data);
