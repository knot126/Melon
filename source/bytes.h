/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * Byte strings
 */

#pragma once

#include "common.h"
#include "error.h"

typedef uint8_t DgByte;

typedef struct DgBytes {
	DgByte *data;
	size_t length;
} DgBytes;

void DgBytesInit(DgBytes *this);
void DgBytesFree(DgBytes *this);
DgByte DgBytesAt_(DgBytes *this, size_t index, const char *debug_file, size_t debug_line);
void DgBytesSet_(DgBytes *this, size_t index, DgByte byte, const char *debug_file, size_t debug_line);
DgError DgBytesAppendBuffer(DgBytes *this, const size_t buffer_length, const void *buffer);
DgError DgBytesAppendByte(DgBytes *this, DgByte byte);
size_t DgBytesLength(DgBytes *this);
DgByte *DgBytesRawBuffer(DgBytes *this);
DgError DgBytesToBuffer(DgBytes *this, DgByte **buffer, size_t *buffer_length);
bool DgBytesEqual(const DgBytes * const restrict bytes1, const DgBytes * const restrict bytes2);
uint64_t DgBytesQuickHash(DgBytes *this);

#define DgBytesAt(THIS, INDEX) DgBytesAt_(THIS, INDEX, __FILE__, __LINE__)
#define DgBytesSet(THIS, INDEX, VALUE) DgBytesSet_(THIS, INDEX, VALUE, __FILE__, __LINE__)
