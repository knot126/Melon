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
#include "log.h"

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
	
	value->type = DG_TYPE_STRING;
	value->flags = 0;
	
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
	value->type = DG_TYPE_STRING;
	value->flags = DG_VALUE_STATIC;
	
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
	
	value->data.asArray = data;
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

/* ************************************************************************** */

DgValue DgMakeNil(void) {
	/**
	 * Make a value of the type Nil.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueNil(&v);
	return v;
}

DgValue DgMakeBool(bool data) {
	/**
	 * Make a value of the type Bool.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueBool(&v, data);
	return v;
}

DgValue DgMakeInt8(int8_t data) {
	/**
	 * Make a value of the type Int8.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueInt8(&v, data);
	return v;
}

DgValue DgMakeUInt8(uint8_t data) {
	/**
	 * Make a value of the type UInt8.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueUInt8(&v, data);
	return v;
}

DgValue DgMakeInt16(int16_t data) {
	/**
	 * Make a value of the type Int16.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueInt16(&v, data);
	return v;
}

DgValue DgMakeUInt16(uint16_t data) {
	/**
	 * Make a value of the type UInt16.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueUInt16(&v, data);
	return v;
}

DgValue DgMakeInt32(int32_t data) {
	/**
	 * Make a value of the type Int32.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueInt32(&v, data);
	return v;
}

DgValue DgMakeUInt32(uint32_t data) {
	/**
	 * Make a value of the type UInt32.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueUInt32(&v, data);
	return v;
}

DgValue DgMakeInt64(int64_t data) {
	/**
	 * Make a value of the type Int64.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueInt64(&v, data);
	return v;
}

DgValue DgMakeUInt64(uint64_t data) {
	/**
	 * Make a value of the type UInt64.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueUInt64(&v, data);
	return v;
}

DgValue DgMakeFloat32(float data) {
	/**
	 * Make a value of the type Float32.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueFloat32(&v, data);
	return v;
}

DgValue DgMakeFloat64(double data) {
	/**
	 * Make a value of the type Float64.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueFloat64(&v, data);
	return v;
}

DgValue DgMakeString(const char * data) {
	/**
	 * Make a value of the type String.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueString(&v, data);
	return v;
}

DgValue DgMakeStaticString(const char * data) {
	/**
	 * Make a value of the type StaticString.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueStaticString(&v, data);
	return v;
}

DgValue DgMakePointer(void * data) {
	/**
	 * Make a value of the type Pointer.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValuePointer(&v, data);
	return v;
}

DgValue DgMakeArray(struct DgArray * data) {
	/**
	 * Make a value of the type Array.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueArray(&v, data);
	return v;
}

DgValue DgMakeTable(struct DgTable * data) {
	/**
	 * Make a value of the type Table.
	 * 
	 * @param Data that the generic value will contain
	 * @return Value
	 */
	
	DgValue v;
	DgValueTable(&v, data);
	return v;
}

/* ************************************************************************** */

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
	
	// TODO Free array
	else if (this->type == DG_TYPE_ARRAY) {
		return DG_ERROR_NOT_IMPLEMENTED;
	}
	
	// Any other case does not need automatic free
	return DG_ERROR_SUCCESSFUL;
}

DgValueType DgValueGetType(DgValue * restrict this) {
	/**
	 * Get the type of value this DgValue object stores
	 * 
	 * @param this Value to get type of
	 * @return Type of value
	 */
	
	return this->type;
}

bool DgValueEqual(const DgValue * const restrict value1, const DgValue * const restrict value2) {
	/**
	 * Check if the two given values are equal.
	 * 
	 * @note Integers of different sizes and/or signs (ex: Int16, UInt32) will
	 * always be treated as being different, even if their actual values are
	 * the same.
	 * 
	 * @param value1 First value
	 * @param value2 Second value
	 * @return If the values are equal or not
	 */
	
	DgTableType type1 = DgValueGetType(value1);
	DgTableType type2 = DgValueGetType(value2);
	
	// The types of both must be the same for equality. Simple!
	if (type1 != type2) {
		return false;
	}
	
	// Compare based on the type
	switch (type1) {
		case DG_TYPE_NIL:
		case DG_TYPE_NULL: {
			return true;
		}
		
		case DG_TYPE_BOOL: { return (value1->data.asBool == value2->data.asBool); }
		
		case DG_TYPE_INT8: { return (value1->data.asInt8 == value2->data.asInt8); }
		case DG_TYPE_UINT8: { return (value1->data.asUInt8 == value2->data.asUInt8); }
		case DG_TYPE_INT16: { return (value1->data.asInt16 == value2->data.asInt16); }
		case DG_TYPE_UINT16: { return (value1->data.asUInt16 == value2->data.asUInt16); }
		case DG_TYPE_INT32: { return (value1->data.asInt32 == value2->data.asInt32); }
		case DG_TYPE_UINT32: { return (value1->data.asUInt32 == value2->data.asUInt32); }
		case DG_TYPE_INT64: { return (value1->data.asInt64 == value2->data.asInt64); }
		case DG_TYPE_UINT64: { return (value1->data.asUInt64 == value2->data.asUInt64); }
		
		case DG_TYPE_FLOAT32: { return (value1->data.asFloat32 == value2->data.asFloat32); }
		case DG_TYPE_FLOAT64: { return (value1->data.asFloat64 == value2->data.asFloat64); }
		
		case DG_TYPE_POINTER: { return (value1->data.asPointer == value2->data.asPointer); }
		
		case DG_TYPE_STRING: { return DgStringEqual(value1->data.asStaticString, value2->data.asStaticString); }
		
		/// @todo DG_TYPE_BYTES, DG_TYPE_ARRAY, DG_TYPE_TABLE
		
		default: {
			DgLog(DG_LOG_WARNING, "DgValueEqual: Equality is not implemented for type <0x%x>!!", type1);
			return false;
		}
	}
	
	// Just in case...
	return false;
}

uint64_t DgValueHash(const DgValue * const restrict this) {
	/**
	 * Get a hash of the given value that can be used for a hash table.
	 * 
	 * @note This only meant to be used by DgTable.
	 * 
	 * @note It's perfectly fine that, for integer values, hash(x) = x. In fact,
	 * for hash tables this is optimal, since this maximises usage of the hash
	 * table's buckets.
	 * 
	 * @see https://en.wikipedia.org/wiki/Hash_table
	 * 
	 * @param this Value to get hash of
	 * @return Hash of value
	 */
	
	DgValueType type = DgValueGetType(this);
	
	switch (this) {
		case DG_TYPE_NIL: { return 0xbadf00d; }
		case DG_TYPE_NULL: { return 0xdeadbeef; }
		
		case DG_TYPE_BOOL: { return this->data.asBool; }
		
		case DG_TYPE_INT8: { return this->data.asUInt8; }
		case DG_TYPE_UINT8: { return this->data.asUInt8; }
		case DG_TYPE_INT16: { return this->data.asUInt16; }
		case DG_TYPE_UINT16: { return this->data.asUInt16; }
		case DG_TYPE_INT32: { return this->data.asUInt32; }
		case DG_TYPE_UINT32: { return this->data.asUInt32; }
		case DG_TYPE_INT64: { return this->data.asUInt64; }
		case DG_TYPE_UINT64: { return this->data.asUInt64; }
		
		case DG_TYPE_FLOAT32: { return this->data.asUInt32; }
		case DG_TYPE_FLOAT64: { return this->data.asUInt64; }
		
		case DG_TYPE_POINTER: { return this->data.asUInt64; }
		
		case DG_TYPE_STRING: { return DgChecksumStringU32_DJB2(this->data.asStaticString); }
		
		/// @todo DG_TYPE_BYTES, DG_TYPE_ARRAY, DG_TYPE_TABLE
		
		default: {
			DgLog(DG_LOG_WARNING, "DgValueHash: Hash is not implemented for type <0x%x>!!", type);
			return 0xdabdab;
		}
	}
}
