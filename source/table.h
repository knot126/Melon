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
 * The structure for each key in the hash table.
 * 
 * NOTE: I'm not yet storing the key here, just seems like it uses too much
 * memory when could you just check the index for a key match :P
 * 
 * NOTE: I could also chose to use DgArray here instead of manually implementing
 * a linked list, I just feel like that could have too much memory overhead when
 * the arrays will be at most 4 - 5 entries long. Then again, it might be more
 * sane and in C you need to value any little bit of sanity because C is trying
 * to murder you at all times and any successful attempts to resist are, of
 * course, beyond what any human should need to endure.
 */
struct DgTableQuick;
typedef struct DgTableQuick {
	size_t index;              // Index into the array of values
	                           // Should have some type of sintel value
	struct DgTableQuick *next; // Next possible key for this hash output
} DgTableQuick;

enum {
	DG_TABLE_LUT_NIL = 0xFFffFFffFFffFFff,
};

/**
 * Real table structure
 */
typedef struct DgTable {
	// First: The array for quick lookups, implemented like a hash table.
	// It will only ever be power of two sizes so don't really need to keep
	// a difference between the allocated and actual lengths :3
	DgTableQuick *lookup;   // Hash table that maps key hashes -> indexes
	size_t lookup_alloc;    // Number of allocated slots in quick lookup table
	                        // = 2 ^ number of bits of the hash to use
	
	// The array of pairs, which is stored as a typical array. In fact, why
	// the fuck shouldn't we just use a DgArray for this? There, I did it. UwU
	DgArray array;
} DgTable;

DgError DgTableInit(DgTable *this);
DgError DgTableFree(DgTable *this, bool deep);

bool DgTableHas(DgTable * restrict this, DgValue * restrict key);
DgError DgTablePut(DgTable * restrict this, DgValue * restrict key, DgValue * restrict value);
DgValue *DgTableAt(DgTable * restrict this, DgValue * restrict key);
DgError DgTableRemove(DgTable * restrict this, const DgValue * const restrict key);
DgError DgTablePairAt(DgTable * restrict this, size_t index, DgValue ** const restrict key, DgValue ** const restrict value);

size_t DgTableLength(DgTable * restrict this);

DgError DgTableSetPointer(DgTable * restrict this, const char *key, void *value);
void *DgTableGetPointer(DgTable * restrict this, const char *key);
