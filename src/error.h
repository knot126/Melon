/**
 * Copyright (C) 2021 - 2022 Knot126
 * =================================
 * 
 * Error handling
 */

#include <inttypes.h>

typedef uint32_t DgError;

enum {
	DG_ERROR_SUCCESSFUL = 0,
	DG_ERROR_OUT_OF_MEMORY = -1,
	DG_ERROR_FILE_NOT_FOUND = -2,
	DG_ERROR_NOT_FOUND = -3,
	DG_ERROR_NO_CLIENT = -4,
};

typedef struct DgErrorCallback {
	
} DgErrorCallback;

bool DgErrorFatal(DgError error);
