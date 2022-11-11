/**
 * Copyright (C) 2022 Knot126
 * ==========================
 * 
 * Map/Table array type
 */

#pragma once

#include <inttypes.h>
#include <stdlib.h>

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
	//DG_TABLE_TYPE_BYTES = 14,
	DG_TABLE_TYPE_FLOAT32 = 15,
	DG_TABLE_TYPE_FLOAT64 = 16,
};

struct DgTableArrayHeader;
struct DgTable;

/**
 * Data value storage type
 */
typedef union DgTableValueData {
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
} DgTableValueData;

/**
 * Value storage type
 */
typedef struct DgTableValue {
	DgTableValueData data; // Raw data bytes
	uint32_t metadata;     // Used for string hash or cached array length
	DgTableType type;      // Type of value stored
	uint16_t flags;        // To be used for flags applied to the value
} DgTableValue;

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
	DgTableValue *key;    // Keys
	DgTableValue *value;  // Values
	size_t length;        // Length of used entries
	size_t allocated;     // Length of allocated entries
} DgTable;

// Table functions
DgError DgTableInit(DgTable *this);
DgError DgTableFree(DgTable *this);

DgError DgTableSet(DgTable * restrict this, DgTableValue * restrict key, DgTableValue * restrict value);
DgError DgTableGet(DgTable * restrict this, DgTableValue * restrict key, DgTableValue * restrict value);
DgError DgTableRemove(DgTable * restrict this, DgTableValue * const restrict key);

// Array functions
// ...

// Value functions
DgError DgValueNil(DgTableValue * restrict value);
DgError DgValueInt8(DgTableValue * restrict value, int8_t data);
DgError DgValueUInt8(DgTableValue * restrict value, uint8_t data);
DgError DgValueInt16(DgTableValue * restrict value, int16_t data);
DgError DgValueUInt16(DgTableValue * restrict value, uint16_t data);
DgError DgValueInt32(DgTableValue * restrict value, int32_t data);
DgError DgValueUInt32(DgTableValue * restrict value, uint32_t data);
DgError DgValueInt64(DgTableValue * restrict value, int64_t data);
DgError DgValueUInt64(DgTableValue * restrict value, uint64_t data);
DgError DgValueFloat32(DgTableValue * restrict value, float data);
DgError DgValueFloat64(DgTableValue * restrict value, double data);
DgError DgValueString(DgTableValue * restrict value, char * restrict data);
DgError DgValueStaticString(DgTableValue * restrict value, const char * restrict data);
DgError DgValuePointer(DgTableValue * restrict value, void *data);
DgError DgValueFree(DgTableValue * restrict this);

bool DgValueEqual(const DgTableValue * const restrict value1, const DgTableValue * const restrict value2);
