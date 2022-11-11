/**
 * Copyright (C) 2021 - 2022 Knot126
 * =================================
 * 
 * Error handling
 */

#pragma once

#include <inttypes.h>
#include <stdbool.h>

typedef int32_t DgError;

enum {
	DG_ERROR_SUCCESSFUL = 0,
	DG_ERROR_SUCCESS = 0,
	
	DG_ERROR_OUT_OF_MEMORY = -0x10,
	DG_ERROR_ALLOCATION_FAILED = -0x11,
	
	DG_ERROR_FILE_NOT_FOUND = -0x20,
	DG_ERROR_NOT_FOUND = -0x21,
	DG_ERROR_NOT_INITIALISED = -0x22,
	DG_ERROR_ALREADY_EXISTS = -0x23,
	DG_ERROR_READ_ONLY = -0x24,
	
	DG_ERROR_NO_CLIENT = -0x30,
	DG_ERROR_REJECTED = -0x31,
	DG_ERROR_FORBIDDEN = -0x32,
	
	DG_ERROR_NOT_IMPLEMENTED = -0x40,
	DG_ERROR_NOT_SUPPORTED = -0x41,
	
	DG_ERROR_FAILED = -0x7fffffff,
};

typedef struct DgErrorCallback {
	int _lel;
} DgErrorCallback;

bool DgErrorFatal(DgError error);
