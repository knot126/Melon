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
	DG_ERROR_OUT_OF_MEMORY = -1,
	DG_ERROR_FILE_NOT_FOUND = -2,
	DG_ERROR_NOT_FOUND = -3,
	DG_ERROR_NO_CLIENT = -4,
	DG_ERROR_NOT_INITIALISED = -5,
	DG_ERROR_NOT_IMPLEMENTED = -6,
	DG_ERROR_ALREADY_EXISTS = -7,
	DG_ERROR_ALLOCATION_FAILED = -8,
};

typedef struct DgErrorCallback {
	int _lel;
} DgErrorCallback;

bool DgErrorFatal(DgError error);
