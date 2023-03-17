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
 * Map/Table array type
 */

#pragma once

#include <stdbool.h>
#include <inttypes.h>
#include <stdlib.h>

#include "error.h"

typedef uint16_t DgTableType;

/**
 * Type IDs
 */
enum {
	DG_TABLE_TYPE_NIL = 0,
	DG_TABLE_TYPE_INT8 = 1,
	DG_TABLE_TYPE_UINT8 = 2,
	DG_TABLE_TYPE_INT16 = 3,
	DG_TABLE_TYPE_UINT16 = 4,
	DG_TABLE_TYPE_INT32 = 5,
	DG_TABLE_TYPE_UINT32 = 6,
	DG_TABLE_TYPE_INT64 = 7,
	DG_TABLE_TYPE_UINT64 = 8,
	DG_TABLE_TYPE_POINTER = 9,
	DG_TABLE_TYPE_ARRAY = 10,
	DG_TABLE_TYPE_TABLE = 11,
	DG_TABLE_TYPE_STRING = 12,
	DG_TABLE_TYPE_STATIC_STRING = 13,
	DG_TABLE_TYPE_DATA = 14,
	DG_TABLE_TYPE_FLOAT32 = 15,
	DG_TABLE_TYPE_FLOAT64 = 16,
	DG_TABLE_TYPE_BOOL = 17,
};

struct DgTableArrayHeader;
struct DgTable;

/**
 * Data value storage type
 */
typedef union DgValueData {
	int8_t asInt8;
	uint8_t asUInt8;
	int16_t asInt16;
	uint16_t asUInt16;
	int32_t asInt32;
	uint32_t asUInt32;
	int64_t asInt64;
	uint64_t asUInt64;
	void *asPointer;
	struct DgTableArrayHeader *asArray;
	struct DgTable *asTable;
	char *asString;
	const char *asStaticString;
	uint8_t *asBytes;
	float asFloat32;
	double asFloat64;
	bool asBool;
} DgValueData;

/**
 * Value storage type
 */
typedef struct DgValue {
	DgValueData data; // Raw data bytes
	uint32_t metadata;     // Used for string hash or cached array length
	DgTableType type;      // Type of value stored
	uint16_t flags;        // To be used for flags applied to the value
} DgValue;

/**
 * Header part of the array memory block
 */
typedef struct DgTableArrayHeader {
	size_t header_size; // Number of padding bytes to skip to get to start of array data
	size_t length;      // Number of elements in array
} DgTableArrayHeader;

/**
 * Actual type for the table
 */
typedef struct DgTable {
	DgValue *key;    // Keys
	DgValue *value;  // Values
	size_t length;        // Length of used entries
	size_t allocated;     // Length of allocated entries
} DgTable;

// Table functions
DgError DgTableInit(DgTable *this);
DgError DgTableFree(DgTable *this);

DgError DgTableSet(DgTable * restrict this, DgValue * restrict key, DgValue * restrict value);
DgError DgTableGet(DgTable * restrict this, DgValue * restrict key, DgValue * restrict value);
DgError DgTableRemove(DgTable * restrict this, DgValue * const restrict key);
DgError DgTableAt(DgTable * restrict this, size_t index, DgValue * const restrict key, DgValue * const restrict value);

// Array functions
// ...

// Value functions
DgError DgValueNil(DgValue * restrict value);
DgError DgValueBool(DgValue * restrict value, bool data);
DgError DgValueInt8(DgValue * restrict value, int8_t data);
DgError DgValueUInt8(DgValue * restrict value, uint8_t data);
DgError DgValueInt16(DgValue * restrict value, int16_t data);
DgError DgValueUInt16(DgValue * restrict value, uint16_t data);
DgError DgValueInt32(DgValue * restrict value, int32_t data);
DgError DgValueUInt32(DgValue * restrict value, uint32_t data);
DgError DgValueInt64(DgValue * restrict value, int64_t data);
DgError DgValueUInt64(DgValue * restrict value, uint64_t data);
DgError DgValueFloat32(DgValue * restrict value, float data);
DgError DgValueFloat64(DgValue * restrict value, double data);
DgError DgValueString(DgValue * restrict value, const char * restrict data);
DgError DgValueStaticString(DgValue * restrict value, const char * restrict data);
DgError DgValuePointer(DgValue * restrict value, void *data);
DgError DgValueTable(DgValue * restrict value, DgTable *data);
DgError DgValueFree(DgValue * restrict this);

bool DgValueEqual(const DgValue * const restrict value1, const DgValue * const restrict value2);
