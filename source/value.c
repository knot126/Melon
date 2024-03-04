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

#include "value.h"

#include "string.h"
#include "checksum.h"
#include "alloc.h"

DgError DgValueNil(DgValue * restrict value) {
	/**
	 * Create a NIL value.
	 * 
	 * @param value Value object
	 * @return Error code
	 */
	
	value->data.asInt64 = 0;
	value->type = DG_TYPE_NIL;
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgValueBool(DgValue * restrict value, bool data) {
	/**
	 * Create a signed 8-bit integer value.
	 * 
	 * @param value Value object
	 * @param data Data value to set to
	 * @return Error code
	 */
	
	value->data.asBool = data;
	value->type = DG_TYPE_BOOL;
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgValueInt8(DgValue * restrict value, int8_t data) {
	/**
	 * Create a signed 8-bit integer value.
	 * 
	 * @param value Value object
	 * @param data Data value to set to
	 * @return Error code
	 */
	
	value->data.asInt8 = data;
	value->type = DG_TYPE_INT8;
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgValueUInt8(DgValue * restrict value, uint8_t data) {
	/**
	 * Create an unsigned 8-bit integer value.
	 * 
	 * @param value Value object
	 * @param data Data value to set to
	 * @return Error code
	 */
	
	value->data.asUInt8 = data;
	value->type = DG_TYPE_UINT8;
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgValueInt16(DgValue * restrict value, int16_t data) {
	/**
	 * Create a signed 16-bit integer value.
	 * 
	 * @param value Value object
	 * @param data Data value to set to
	 * @return Error code
	 */
	
	value->data.asInt16 = data;
	value->type = DG_TYPE_INT16;
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgValueUInt16(DgValue * restrict value, uint16_t data) {
	/**
	 * Create an unsigned 16-bit integer value.
	 * 
	 * @param value Value object
	 * @param data Data value to set to
	 * @return Error code
	 */
	
	value->data.asUInt16 = data;
	value->type = DG_TYPE_UINT16;
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgValueInt32(DgValue * restrict value, int32_t data) {
	/**
	 * Create a signed 32-bit integer value.
	 * 
	 * @param value Value object
	 * @param data Data value to set to
	 * @return Error code
	 */
	
	value->data.asInt32 = data;
	value->type = DG_TYPE_INT32;
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgValueUInt32(DgValue * restrict value, uint32_t data) {
	/**
	 * Create an unsigned 32-bit integer value.
	 * 
	 * @param value Value object
	 * @param data Data value to set to
	 * @return Error code
	 */
	
	value->data.asUInt32 = data;
	value->type = DG_TYPE_UINT32;
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgValueInt64(DgValue * restrict value, int64_t data) {
	/**
	 * Create a signed 64-bit integer value.
	 * 
	 * @param value Value object
	 * @param data Data value to set to
	 * @return Error code
	 */
	
	value->data.asInt64 = data;
	value->type = DG_TYPE_INT64;
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgValueUInt64(DgValue * restrict value, uint64_t data) {
	/**
	 * Create an unsigned 64-bit integer value.
	 * 
	 * @param value Value object
	 * @param data Data value to set to
	 * @return Error code
	 */
	
	value->data.asUInt64 = data;
	value->type = DG_TYPE_UINT64;
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgValueFloat32(DgValue * restrict value, float data) {
	/**
	 * Create a 32-bit float value.
	 * 
	 * @param value Value object
	 * @param data Data value to set to
	 * @return Error code
	 */
	
	value->data.asFloat32 = data;
	value->type = DG_TYPE_FLOAT32;
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgValueFloat64(DgValue * restrict value, double data) {
	/**
	 * Create a 64-bit float value.
	 * 
	 * @param value Value object
	 * @param data Data value to set to
	 * @return Error code
	 */
	
	value->data.asFloat64 = data;
	value->type = DG_TYPE_FLOAT64;
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgValueString(DgValue * restrict value, const char * restrict data) {
	/**
	 * Create a regular string value.
	 * 
	 * @note This allocates its own copy of string memory and must be freed
	 * at some point. Many functions free values automatically.
	 * 
	 * @param value Value object
	 * @param data Data value to set to
	 * @return Error code
	 */
	
	value->data.asString = DgStringDuplicate(data);
	
	if (value->data.asString == NULL) {
		return DG_ERROR_ALLOCATION_FAILED;
	}
	
	value->metadata = DgChecksumStringU32(value->data.asString);
	value->type = DG_TYPE_STRING;
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgValueStaticString(DgValue * restrict value, const char * restrict data) {
	/**
	 * Create a static string value.
	 * 
	 * @note This does not allocate its own copy of string memory
	 * 
	 * @param value Value object
	 * @param data Data value to set to
	 * @return Error code
	 */
	
	value->data.asStaticString = data;
	value->metadata = DgChecksumStringU32(value->data.asStaticString);
	value->type = DG_TYPE_STATIC_STRING;
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgValuePointer(DgValue * restrict value, void *data) {
	/**
	 * Create a pointer value.
	 * 
	 * @param value Value object
	 * @param data Data value to set to
	 * @return Error code
	 */
	
	value->data.asPointer = data;
	value->type = DG_TYPE_POINTER;
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgValueArray(DgValue * restrict value, struct DgArray *data) {
	/**
	 * Create an array value.
	 * 
	 * @param value Value object
	 * @param data Data value to set to
	 * @return Error code
	 */
	
	value->data.asTable = data;
	value->type = DG_TYPE_ARRAY;
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgValueTable(DgValue * restrict value, struct DgTable *data) {
	/**
	 * Create a table value.
	 * 
	 * @param value Value object
	 * @param data Data value to set to
	 * @return Error code
	 */
	
	value->data.asTable = data;
	value->type = DG_TYPE_TABLE;
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgTableFree(struct DgTable *this);

DgError DgValueFree(DgValue * restrict this) {
	/**
	 * Release memory assocaited with a value (if needed)
	 * 
	 * @param this Value
	 * @return Error code
	 */
	
	// Free non-static string
	if ((this->type == DG_TYPE_STRING) && (this->data.asString)) {
		DgMemoryFree(this->data.asString);
		return DG_ERROR_SUCCESSFUL;
	}
	
	// Free sub-table
	else if (this->type == DG_TYPE_TABLE) {
		return DgTableFree(this->data.asTable);
	}
	
	// Free array
	else if (this->type == DG_TYPE_ARRAY) {
		return DG_ERROR_NOT_IMPLEMENTED;
	}
	
	// Any other case does not need automatic free
	return DG_ERROR_SUCCESSFUL;
}

bool DgValueEqual(const DgValue * const restrict value1, const DgValue * const restrict value2) {
	/**
	 * Check if the two given values are equal.
	 * 
	 * @note String and Static String with the same data will compare the same.
	 * 
	 * @param value1 First value
	 * @param value2 Second value
	 * @return If the values are equal or not
	 */
	
	DgTableType type1 = value1->type;
	DgTableType type2 = value2->type;
	
	// Let strings and static strings be the same type for comparison
	if (type1 == DG_TYPE_STATIC_STRING) { type1 = DG_TYPE_STRING; }
	if (type2 == DG_TYPE_STATIC_STRING) { type2 = DG_TYPE_STRING; }
	
	// The types of both now must be the same
	if (type1 != type2) {
		return false;
	}
	
	// Compare based on the type
	switch (type1) {
		case DG_TYPE_STRING: {
			return ((value1->metadata == value2->metadata) && (DgStringEqual(value1->data.asStaticString, value2->data.asStaticString)));
		}
		
		default: {
			/// @warning This does not work with non-64-bit values
			return (value1->data.asUInt64 == value2->data.asUInt64);
		}
	}
	
	// Just in case...
	return false;
}
