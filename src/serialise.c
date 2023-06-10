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

#include "serialise.h"

DgError DgSerialiseWrite(DgStorage *storage, const char *path, DgTable *table) {
	/**
	 * Write the table to the given file path.
	 * 
	 * @param storage Storage object to use
	 * @param path Path to write to
	 * @param table Table data to write
	 * @return Error status
	 */
	
	DgStream stream;
	DgError status = DgStreamOpen(storage, &stream, path, 0);
	
	if (status != DG_ERROR_SUCCESS) {
		return status;
	}
	
	DgStreamWriteUInt32(&stream, 0xFC991E51);
	
	DgStreamClose(&stream);
	
	return DG_ERROR_FAILED;
}
