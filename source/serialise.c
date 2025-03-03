/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Serialisation for tables (custom binary format)
 */

#include "machine.h"
#include "stream.h"
#include "bytes.h"
#include "array.h"
#include "table.h"
#include "error.h"
#include "log.h"

#include "serialise.h"

#define CHECK_STATUS(status, onfail) if (status) { goto onfail; }

DgError DgSerialiseWriteValue(DgStream * restrict stream, const DgValue * restrict value) {
	/**
	 * Write out a value
	 * 
	 * @param stream Stream to serialise the value to
	 * @param value Value to write
	 * @return Error code
	 */
	
	DgError status;
	
	// Temp store the type
	DgValueType type = value->type;
	
	// Write the type ID
	status = DgStreamWriteLEB128(stream, type);
	
	if (status) {
		return status;
	}
	
	// Write the value
	switch (type) {
		case DG_TYPE_NIL:
			break;
		case DG_TYPE_BOOL:
			status = DgStreamWriteInt8(stream, value->data.asBool);
			break;
		case DG_TYPE_INT64:
			// Signed int's are not yet supported LEB in 2.0 :(
			status = DgStreamWriteInt64(stream, value->data.asInt64);
			break;
		case DG_TYPE_UINT64:
			status = DgStreamWriteLEB128(stream, value->data.asUInt64);
			break;
		case DG_TYPE_STRING:
			status = DgStreamWriteString(stream, value->data.asStaticString);
			if (!status) { status = DgStreamWriteInt8(stream, 0); } // NUL terminator
			break;
		case DG_TYPE_FLOAT32:
			status = DgStreamWriteFloat32(stream, value->data.asFloat32);
			break;
		case DG_TYPE_FLOAT64:
			status = DgStreamWriteFloat64(stream, value->data.asFloat64);
			break;
		case DG_TYPE_BYTES: {
			DgBytes *bytes = value->data.asBytes;
			
			size_t length = DgBytesLength(bytes);
			DgByte *data = DgBytesRawBuffer(bytes);
			
			status = DgStreamWriteUInt64(stream, length);
			
			if (!status) {
				status = DgStreamWrite(stream, length, data);
			}
			
			break;
		}
		case DG_TYPE_ARRAY: {
			DgArray *array = value->data.asArray;
			size_t length = DgArrayLength(array);
			
			status = DgStreamWriteLEB128(stream, length);
			
			if (status) {
				return status;
			}
			
			for (size_t i = 0; i < length; i++) {
				DgValue *item = DgArrayAt(array, i);
				
				status = DgSerialiseWriteValue(stream, item);
				
				if (status) {
					return status;
				}
			}
			
			break;
		}
		case DG_TYPE_TABLE: {
			DgTable *table = value->data.asTable;
			size_t length = DgTableLength(table);
			
			status = DgStreamWriteLEB128(stream, length);
			
			if (status) {
				return status;
			}
			
			for (size_t i = 0; i < length; i++) {
				DgValue *key, *value;
				DgTablePairAt(table, i, &key, &value);
				
				status = DgSerialiseWriteValue(stream, key);
				status |= DgSerialiseWriteValue(stream, value); // HACK im lazy
				
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

DgError DgSerialiseWrite(DgStream *stream, DgValue * restrict value) {
	/**
	 * Write the value to the given file path.
	 * 
	 * @param stream Stream to write data to
	 * @param value Value to write
	 * @return Error status
	 */
	
	// Open stream
	DgStreamSetEndian(stream, DG_ENDIAN_LITTLE);
	
	// Magic number
	DgError status = DgStreamWriteUInt32(stream, 0xFC991E51); // FURRIES!
	CHECK_STATUS(status, onfail);
	
	// Version
	status = DgStreamWriteLEB128(stream, 2); CHECK_STATUS(status, onfail);
	status = DgStreamWriteLEB128(stream, 0); CHECK_STATUS(status, onfail);
	
	// Serialise root value
	status = DgSerialiseWriteValue(stream, value);
	
	onfail:
	return status;
}
