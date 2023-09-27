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

#pragma once

#include <inttypes.h>
#include <stdbool.h>

typedef int32_t DgError;

enum {
	// Successful
	DG_ERROR_SUCCESSFUL = 0,
	DG_ERROR_SUCCESS = 0,
	
	DG_ERROR_OUT_OF_MEMORY = -0x10, // Out of memory (only when really out)
	DG_ERROR_ALLOCATION_FAILED = -0x11, // Allocation of memory failed
	
	DG_ERROR_FILE_NOT_FOUND = -0x20, // Did not find the file
	DG_ERROR_NOT_FOUND = -0x21,
	DG_ERROR_NOT_INITIALISED = -0x22, // Something was not initialised
	DG_ERROR_ALREADY_EXISTS = -0x23,
	DG_ERROR_READ_ONLY = -0x24,
	DG_ERROR_NOT_SAFE = -0x25, // Not safe to preform the operation (e.g. NULL)
	DG_ERROR_WRITE_FAILED = -0x26,
	
	DG_ERROR_NO_CLIENT = -0x30,
	DG_ERROR_REJECTED = -0x31,
	DG_ERROR_FORBIDDEN = -0x32,
	
	DG_ERROR_NOT_IMPLEMENTED = -0x40, // Feature not implemented yet
	DG_ERROR_NOT_SUPPORTED = -0x41, // Platform does not support this feature
	
	DG_ERROR_FAILED = -0x7fffffff,
};

typedef struct DgErrorCallback {
	int _lel;
} DgErrorCallback;

bool DgErrorFatal(DgError error);
const char *DgErrorString(const DgError error);
DgError DgLogError_(const DgError error, const char * const path, const int line);
#define DgLogError(error) DgLogError_(error, __FILE__, __LINE__);
