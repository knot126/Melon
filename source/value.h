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
 * Generic value variables
 */

#pragma once

#include "common.h"
#include "error.h"
#include "bytes.h"

typedef uint32_t DgValueType;
typedef uint32_t DgValueFlags;

/**
 * Type IDs
 */
enum {
	// None class
	DG_TYPE_NIL = 0x01,
	DG_TYPE_NULL = 0x02,
	// Boolean class
	DG_TYPE_BOOL = 0x11,
	// Integer class
	DG_TYPE_INT8 = 0x21,
	DG_TYPE_UINT8 = 0x22,
	DG_TYPE_INT16 = 0x23,
	DG_TYPE_UINT16 = 0x24,
	DG_TYPE_INT32 = 0x25,
	DG_TYPE_UINT32 = 0x26,
	DG_TYPE_INT64 = 0x27,
	DG_TYPE_UINT64 = 0x28,
	// Rational class
	DG_TYPE_FLOAT32 = 0x31,
	DG_TYPE_FLOAT64 = 0x32,
	// Low level data class
	DG_TYPE_POINTER = 0x41,
	// Value terminated array class
	DG_TYPE_STRING = 0x51,
	DG_TYPE_STATIC_STRING = 0x52,
	// High level class
	DG_TYPE_BYTES = 0x61, // coming soon
	DG_TYPE_ARRAY = 0x62,
	DG_TYPE_TABLE = 0x63,
};

/**
 * Value flags
 */
enum {
	DG_VALUE_STATIC = (1 << 0),
};

struct DgArray;
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
	struct DgArray *asArray;
	struct DgTable *asTable;
	DgBytes *asBytes;
	char *asString;
	const char *asStaticString;
	uint8_t *asRawBytes;
	float asFloat32;
	double asFloat64;
	bool asBool;
} DgValueData;

/**
 * Value storage type
 */
typedef struct DgValue {
	DgValueData data; // Raw data bytes
	DgValueType type;      // Type of value stored
	DgValueFlags flags;        // To be used for flags applied to the value
} DgValue;

/**
 * Initialiser functions
 * =====================
 * 
 * These are convinece functions that create values of various types. DgValueXX
 * assumes that there is already memory to store a value; DgMakeXX does not.
 * 
 * @note We don't respect the typical function naming rule of Dg<Module>XX since
 * these will be used a lot and I think the shorter name is preferable.
 */
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
DgError DgValueArray(DgValue * restrict value, struct DgArray *data);
DgError DgValueTable(DgValue * restrict value, struct DgTable *data);

DgValue DgMakeNil(void);
DgValue DgMakeBool(bool data);
DgValue DgMakeInt8(int8_t data);
DgValue DgMakeUInt8(uint8_t data);
DgValue DgMakeInt16(int16_t data);
DgValue DgMakeUInt16(uint16_t data);
DgValue DgMakeInt32(int32_t data);
DgValue DgMakeUInt32(uint32_t data);
DgValue DgMakeInt64(int64_t data);
DgValue DgMakeUInt64(uint64_t data);
DgValue DgMakeFloat32(float data);
DgValue DgMakeFloat64(double data);
DgValue DgMakeString(const char * data);
DgValue DgMakeStaticString(const char * data);
DgValue DgMakePointer(void * data);
DgValue DgMakeArray(struct DgArray * data);
DgValue DgMakeTable(struct DgTable * data);

DgError DgValueFree(DgValue * restrict this);

DgValueType DgValueGetType(const DgValue * const restrict this);
bool DgValueEqual(const DgValue * const restrict value1, const DgValue * const restrict value2);
uint64_t DgValueHash(const DgValue * const restrict this);
