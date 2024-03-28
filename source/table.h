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

/**
 * The structure for each key in the hash table.
 */
struct DgTableQuick;
typedef struct DgTableQuick {
	size_t index;              // Index into the array of values
	                           // Should have some type of sintel value
	struct DgTableQuick *next; // Next possible key for this hash output
} DgTableQuick;

/**
 * The real key/value pair information
 */
typedef struct DgTablePair {
	DgValue value;
	DgValue key;
} DgTablePair;

/**
 * Real table structure
 */
typedef struct DgTable_New {
	DgTableQuick *quick;   // Hash table that maps key hashes -> indexes
	size_t quick_alloc;    // Number of allocated slots in quick table
	                       // = 2 to the number of bits of the hash to use
	
	DgTablePair *pairs;    // Key-value pairs
	size_t pairs_length;   // Count of currently in use slots for pairs
	size_t pairs_alloc;    // Count of currently allocated slots for pairs
} DgTable_New;

/**
 * Actual type for the table
 */
typedef struct DgTable {
	DgValue *key;    // Keys
	DgValue *value;  // Values
	size_t length;        // Length of used entries
	size_t allocated;     // Length of allocated entries
} DgTable;

DgError DgTableInit(DgTable *this);
DgError DgTableFree(DgTable *this);

DgError DgTableSet(DgTable * restrict this, DgValue * restrict key, DgValue * restrict value);
DgError DgTableGet(DgTable * restrict this, DgValue * restrict key, DgValue * restrict value);
DgError DgTableRemove(DgTable * restrict this, DgValue * const restrict key);
DgError DgTableAt(DgTable * restrict this, size_t index, DgValue * const restrict key, DgValue * const restrict value);
size_t DgTableLength(DgTable * restrict this);
