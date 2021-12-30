/*
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * Generic Stream Utilites
 */

#pragma once

#include <stdbool.h>
#include <inttypes.h>

typedef enum DgStreamEnum {
	/* errors */
	DG_STRM_OKAY = 0,
	DG_STRM_ALLOC_ERROR = 1,
	DG_STRM_NO_SPACE = 2,
	DG_STRM_INVALID = 3,
	DG_STRM_UNKNOWN = 4,
	DG_STRM_EOF = 5,
	DG_STRM_RANGE = 6,
	
	/* offsets */
	DG_STRM_CUR = 0,
	DG_STRM_SET = 1,
	DG_STRM_END = 2,
} DgStreamEnum;

typedef struct DgStream {
	/* For storing data */
	uint8_t *data;
	size_t allocated;
	size_t size;
	size_t head;
	
	/* Errors */
	size_t error;
} DgStream;

DgStream *DgStreamCreate(void);
DgStream *DgStreamFromBuffer(void *buffer, size_t size);

void DgStreamFree(DgStream *stream);
void DgBufferFromStream(DgStream *stream, void **pointer, size_t *size);

size_t DgStreamError(DgStream *stream);

size_t DgStreamGetpos(DgStream *stream);
size_t DgStreamLength(DgStream *stream);
void DgStreamSetpos(DgStream *stream, DgStreamEnum offset, int64_t pos);
void DgStreamRewind(DgStream *stream);

void DgStreamRead(DgStream *stream, size_t size, void *buffer);
void DgStreamWrite(DgStream *stream, size_t size, void *buffer);

int8_t DgStreamReadInt8(DgStream *stream);
uint8_t DgStreamReadUInt8(DgStream *stream);
int16_t DgStreamReadInt16(DgStream *stream);
uint16_t DgStreamReadUInt16(DgStream *stream);
int32_t DgStreamReadInt32(DgStream *stream);
uint32_t DgStreamReadUInt32(DgStream *stream);
int64_t DgStreamReadInt64(DgStream *stream);
uint64_t DgStreamReadUInt64(DgStream *stream);
float DgStreamReadFloat(DgStream *stream);
double DgStreamReadDouble(DgStream *stream);

void DgStreamWriteInt8(DgStream *stream, int8_t *data);
void DgStreamWriteUInt8(DgStream *stream, uint8_t *data);
void DgStreamWriteInt16(DgStream *stream, int16_t *data);
void DgStreamWriteUInt16(DgStream *stream, uint16_t *data);
void DgStreamWriteInt32(DgStream *stream, int32_t *data);
void DgStreamWriteUInt32(DgStream *stream, uint32_t *data);
void DgStreamWriteInt64(DgStream *stream, int64_t *data);
void DgStreamWriteUInt64(DgStream *stream, uint64_t *data);
void DgStreamWriteFloat(DgStream *stream, float *data);
void DgStreamWriteDouble(DgStream *stream, double *data);
