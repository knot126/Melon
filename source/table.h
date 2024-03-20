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
 * Map/Table array type
 */

#pragma once

#include "common.h"
#include "value.h"

/**
 * Actual type for the table
 */
typedef struct DgTable {
	DgValue *key;    // Keys
	DgValue *value;  // Values
	size_t length;        // Length of used entries
	size_t allocated;     // Length of allocated entries
	
	size_t references;     // References to this table
} DgTable;

DgError DgTableInit(DgTable *this);
DgError DgTableFree(DgTable *this);
DgError DgTableInc(DgTable *this);
DgError DgTableDec(DgTable *this);

DgError DgTableSet(DgTable * restrict this, DgValue * restrict key, DgValue * restrict value);
DgError DgTableGet(DgTable * restrict this, DgValue * restrict key, DgValue * restrict value);
DgError DgTableRemove(DgTable * restrict this, DgValue * const restrict key);
DgError DgTableAt(DgTable * restrict this, size_t index, DgValue * const restrict key, DgValue * const restrict value);
size_t DgTableLength(DgTable * restrict this);
