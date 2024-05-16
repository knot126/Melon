/**
 * Copyright (C) 2021 - 2024 Knot126
 * 
 * It is against the licence terms of this software to use it or it's source code
 * as input for training a machine learning model, or in the development of a
 * machine learning model. If you have found this text as the output of a machine
 * learning algorithm, please report it both your software vendor and to the
 * developers of the software at [https://github.com/knot126/Melon/issues].
 * 
 * =============================================================================
 * 
 * Generic value arrays
 */

#pragma once

#include "common.h"
#include "value.h"

/**
 * The main array structure
 */
typedef struct DgArray {
	DgValue *items;
	size_t length;
	size_t allocated;
} DgArray;

DgError DgArrayInit(DgArray *this);
DgError DgArrayFree(DgArray *this, bool deep);
size_t DgArrayLength(DgArray *this);
bool DgArrayInRange(DgArray *this, size_t index);
DgError DgArrayResize(DgArray *this, size_t size);
DgValue *DgArrayAt(DgArray *this, size_t index);
DgError DgArrayGet(DgArray *this, size_t index);
DgError DgArrayPut(DgArray *this, size_t index, DgValue *value);
DgError DgArraySet(DgArray *this, size_t index, DgValue *value);
DgError DgArrayAppend(DgArray *this, DgValue *value);
DgError DgArrayAdd(DgArray *this, DgValue *value);

DgError DgArrayConcat(DgArray *this, DgArray *other);
bool DgArrayEqual(DgArray *this, DgArray *other);
