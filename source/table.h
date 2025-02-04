/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Table/Dictionary type
 * 
 * @note This is implemented using a hash table that respects the order mappings
 * were inserted. Its implementation is similar to python in some ways.
 * 
 * To preserve order while still using a hash table, the actual keys and values
 * are kept in an array, and the hash table maps key hashes -> indexes to real
 * key-value pairs. Seprate chaining (a linked list) is used to resolve
 * collisions.
 */

#pragma once

#include "common.h"
#include "value.h"
#include "array.h"

/**
 * Real table structure
 */
typedef struct DgTable {
	// The array of pairs, which is stored as a typical array. In fact, why
	// the fuck shouldn't we just use a DgArray for this? Actually, I did it.
	// UwU
	DgArray array;
} DgTable;

DgError DgTableInit(DgTable *this);
DgError DgTableFree(DgTable *this, bool deep);

bool DgTableHas(DgTable * restrict this, DgValue * restrict key);
DgError DgTablePut(DgTable * restrict this, DgValue * restrict key, DgValue * restrict value);
DgValue *DgTableAt(DgTable * restrict this, DgValue * restrict key);
DgError DgTableRemove(DgTable * restrict this, DgValue * restrict key);
DgError DgTablePairAt(DgTable * restrict this, size_t index, DgValue ** const restrict key, DgValue ** const restrict value);

size_t DgTableLength(DgTable * restrict this);

DgError DgTableSetPointer(DgTable * restrict this, const char *key, void *value);
void *DgTableGetPointer(DgTable * restrict this, const char *key);
