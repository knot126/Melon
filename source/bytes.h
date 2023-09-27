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
 * Byte strings
 */

#pragma once

#include <inttypes.h>

typedef uint8_t DgByte;

typedef struct DgBytes {
	size_t length;
	DgByte *data;
} DgBytes;

void DgBytesInit(DgBytes *this);
void DgBytesFree(DgBytes *this);
DgByte DgBytesAt_(DgBytes *this, size_t index, const char *debug_file, size_t debug_line);
void DgBytesSet_(DgBytes *this, size_t index, DgByte byte, const char *debug_file, size_t debug_line);

#define DgBytesAt(THIS, INDEX) DgBytesAt_(THIS, INDEX, __FILE__, __LINE__)
#define DgBytesSet(THIS, INDEX, VALUE) DgBytesSet_(THIS, INDEX, VALUE, __FILE__, __LINE__)
