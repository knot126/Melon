/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
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
DgError DgArrayRemoveND(DgArray *this, size_t index, size_t amount, bool deep);
DgError DgArrayRemove(DgArray *this, size_t index);

DgError DgArrayConcat(DgArray *this, DgArray *other);
bool DgArrayEqual(DgArray *this, DgArray *other);
