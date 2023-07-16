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
 * Error handling
 */

#include <stdbool.h>
#include "log.h"

#include "error.h"

bool DgErrorFatal(DgError error) {
	/**
	 * Return if the error is fatal or not.
	 */
	
	return (error != DG_ERROR_SUCCESSFUL);
}

const char *DgErrorString(const DgError error) {
	const char *s;
	
	switch (error) {
		case DG_ERROR_SUCCESSFUL       : s = "Success"; break;
		case DG_ERROR_OUT_OF_MEMORY    : s = "Out of memory"; break;
		case DG_ERROR_ALLOCATION_FAILED: s = "Allocation failed"; break;
		case DG_ERROR_FILE_NOT_FOUND   : s = "File not found"; break;
		case DG_ERROR_NOT_FOUND        : s = "Not found"; break;
		case DG_ERROR_NOT_INITIALISED  : s = "Not initialised"; break;
		case DG_ERROR_ALREADY_EXISTS   : s = "Already exists"; break;
		case DG_ERROR_READ_ONLY        : s = "Read only"; break;
		case DG_ERROR_NOT_SAFE         : s = "Not safe"; break;
		case DG_ERROR_NO_CLIENT        : s = "No client"; break;
		case DG_ERROR_REJECTED         : s = "Rejected"; break;
		case DG_ERROR_FORBIDDEN        : s = "Forbidden"; break;
		case DG_ERROR_NOT_IMPLEMENTED  : s = "Not implemented"; break;
		case DG_ERROR_NOT_SUPPORTED    : s = "Not supported"; break;
		case DG_ERROR_FAILED           : s = "Failed"; break;
		default: s = "Unknown type of error"; break;
	}
	
	return s;
}

DgError DgLogError_(const DgError error, const char * const path, const int line) {
	/**
	 * If there was an error, log it.
	 * 
	 * @param error Error code
	 * @param path File path
	 * @param line Line number
	 * @return Error code
	 */
	
	if (error) {
		DgLog(DG_LOG_ERROR, "%s line %d [0x%x] %s", path, line, error, DgErrorString(error));
	}
	
	return error;
}
