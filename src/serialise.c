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
 * Serialisation for tables (custom binary format)
 */

#include "storage.h"
#include "table.h"
#include "error.h"
#include "log.h"

#include "serialise.h"

DgError DgSerialiseWriteValue(DgStream * restrict stream, const DgValue * restrict value) {
	/**
	 * Write out a value
	 * 
	 * @param stream Stream to serialise the value to
	 * @param value Value to write
	 * @return Error code
	 */
	
	DgError status;
	
	DgValueType type = DG_TABLE_TYPE_NIL;
	
	// Some types cannot be serialised in a way that makes sense. For static
	// strings, it's better just to treat them as strings, and for pointers it
	// makes no sense to store them since they will likely change by the time
	// they are deserialised.
	switch (value->type) {
		case DG_TABLE_TYPE_STATIC_STRING: type = DG_TABLE_TYPE_STRING; break;
		case DG_TABLE_TYPE_POINTER: type = DG_TABLE_TYPE_NIL; break;
		default: type = value->type; break;
	}
	
	// Write the type ID
	status = DgStreamWriteUInt16(stream, type);
	
	if (status) {
		return status;
	}
	
	// Write the value
	switch (value->type) {
		case DG_TABLE_TYPE_NIL:
			break;
		case DG_TABLE_TYPE_BOOL:
			status = DgStreamWriteInt8(stream, value->data.asBool);
			break;
		case DG_TABLE_TYPE_INT8:
			status = DgStreamWriteInt8(stream, value->data.asInt8);
			break;
		case DG_TABLE_TYPE_UINT8:
			status = DgStreamWriteUInt8(stream, value->data.asUInt8);
			break;
		case DG_TABLE_TYPE_INT16:
			status = DgStreamWriteInt16(stream, value->data.asInt16);
			break;
		case DG_TABLE_TYPE_UINT16:
			status = DgStreamWriteUInt16(stream, value->data.asUInt16);
			break;
		case DG_TABLE_TYPE_INT32:
			status = DgStreamWriteInt32(stream, value->data.asInt32);
			break;
		case DG_TABLE_TYPE_UINT32:
			status = DgStreamWriteUInt32(stream, value->data.asUInt32);
			break;
		case DG_TABLE_TYPE_INT64:
			status = DgStreamWriteInt64(stream, value->data.asInt64);
			break;
		case DG_TABLE_TYPE_UINT64:
			status = DgStreamWriteUInt64(stream, value->data.asUInt64);
			break;
		case DG_TABLE_TYPE_STRING:
			status = DgStreamWriteString(stream, value->data.asStaticString);
			break;
		case DG_TABLE_TYPE_FLOAT32:
			status = DgStreamWriteFloat32(stream, value->data.asFloat32);
			break;
		case DG_TABLE_TYPE_FLOAT64:
			status = DgStreamWriteFloat64(stream, value->data.asFloat64);
			break;
		case DG_TABLE_TYPE_TABLE: {
			DgTable *table = value->data.asTable;
			size_t length = DgTableLength(table);
			
			status = DgStreamWriteUInt64(stream, length);
			
			if (status) {
				return status;
			}
			
			for (size_t i = 0; i < length; i++) {
				DgValue key = {}, value = {};
				DgTableAt(table, i, &key, &value);
				
				status = DgSerialiseWriteValue(stream, &key);
				status |= DgSerialiseWriteValue(stream, &value); // HACK im lazy
				
				if (status) {
					return status;
				}
			}
			
			break;
		}
		default:
			DgLog(DG_LOG_ERROR, "Trying to serialise unknown value type");
			return DG_ERROR_FAILED;
			break;
	}
	
	return status;
}

DgError DgSerialiseWrite(DgStorage *storage, const char *path, DgValue * restrict value) {
	/**
	 * Write the value to the given file path.
	 * 
	 * @param storage Storage object to use
	 * @param path Path to write to
	 * @param value Value to write
	 * @return Error status
	 */
	
	// Open stream
	DgStream stream;
	DgError status = DgStreamOpen(storage, &stream, path, DG_STREAM_WRITE);
	
	if (status != DG_ERROR_SUCCESS) {
		return status;
	}
	
	// Magic number
	DgStreamWriteUInt32(&stream, 0xFC991E51); // FURRIES!
	
	// Version
	DgStreamWriteUInt16(&stream, 1);
	DgStreamWriteUInt16(&stream, 0);
	
	// Serialise root value
	DgSerialiseWriteValue(&stream, value);
	
	// Close stream
	DgStreamClose(&stream);
	
	return DG_ERROR_FAILED;
}
