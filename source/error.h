/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * Error handling
 */

#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include <setjmp.h>
#include <stdlib.h>

typedef int32_t DgErrorCode;
typedef DgErrorCode DgError;

enum {
	// Successful
	DG_ERROR_SUCCESSFUL = 0,
	DG_ERROR_SUCCESS = 0,
	DG_SUCCESS = 0,
	
	// Nonfatal errors
	DG_ERROR_WAITING = 0x10,
	
	// Possibly fatal errors
	DG_ERROR_OUT_OF_MEMORY = -0x10, // Out of memory (only when really out)
	DG_ERROR_ALLOCATION_FAILED = -0x11, // Allocation of memory failed
	
	DG_ERROR_FILE_NOT_FOUND = -0x20, // Did not find the file
	DG_ERROR_NOT_FOUND = -0x21,
	DG_ERROR_NOT_INITIALISED = -0x22, // Something was not initialised
	DG_ERROR_ALREADY_EXISTS = -0x23,
	DG_ERROR_READ_ONLY = -0x24,
	DG_ERROR_NOT_SAFE = -0x25, // Not safe to preform the operation (e.g. NULL)
	DG_ERROR_WRITE_FAILED = -0x26,
	DG_ERROR_OUT_OF_RANGE = -0x27,
	
	DG_ERROR_NO_CLIENT = -0x30,
	DG_ERROR_REJECTED = -0x31,
	DG_ERROR_FORBIDDEN = -0x32,
	
	DG_ERROR_NOT_IMPLEMENTED = -0x40, // Feature not implemented yet
	DG_ERROR_NOT_SUPPORTED = -0x41, // Platform does not support this feature
	
	DG_ERROR_NAME_LOOKUP_FAILED = -0x50,
	DG_ERROR_CONNECTION_FAILED = -0x51,
	
	DG_ERROR_FAILED = -0x7fffffff,
	DG_FAIL = -0x7fffffff,
};

// Code-based errors
bool DgErrorFatal(DgErrorCode error);
const char *DgErrorString(const DgErrorCode error);
DgError DgLogError_(const DgErrorCode error, const char * const path, const int line);
#define DgLogError(error) DgLogError_(error, __FILE__, __LINE__);

/***
 * Guard/raise-based errors
 */
typedef struct DgErrorInfo {
	const char *type;
	const char *message;
	const char *file;
	const char *function;
	size_t line;
} DgErrorInfo;

typedef struct {
	jmp_buf env;
} DgErrorGuardEntry;

// Primitives
DgErrorGuardEntry *DgGuardNextSlot_(void);
DgErrorInfo *DgGuard_(int status);
void DgRaise_(DgErrorInfo ei);

/**
 * Error rasing and guarding functions/macros
 * ==========================================
 * 
 * NOTE: DgRaise can be temporarily enabled or disabled, so your function
 * *should* still do something after DgRaise that returns an error value if you
 * are writing a library.
 */

#define /* (DgErrorInfo *) */ DgGuard() ( DgGuard_(setjmp(DgGuardNextSlot_()->env)) )
void DgUnguard(void);
#define /* (void) */ DgRaise(TYPE, MESSAGE) ( DgRaise_((DgErrorInfo) {.type = TYPE, .message = MESSAGE, .file = __FILE__, .function = __FUNCTION__, .line = __LINE__}) )
void DgReraise(void);

void DgErrorSetRaiseEnabled(bool enabled);
bool DgErrorGetRaiseEnabled(void);

#define DgTry(TO_TRY, AS_E, TO_CATCH) { DgErrorInfo *AS_E = DgGuard(); if (error_info) {TO_CATCH} else {TO_TRY; DgUnguard();} }
