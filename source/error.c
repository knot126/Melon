/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * Error handling
 */

#include <stdbool.h>
#include <setjmp.h>
#include "log.h"

#include "error.h"

/** Error codes **/

bool DgErrorFatal(DgErrorCode error) {
	/**
	 * Return if the error is fatal or not.
	 */
	
	return (error != DG_ERROR_SUCCESSFUL);
}

const char *DgErrorString(const DgErrorCode error) {
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
		case DG_ERROR_OUT_OF_RANGE     : s = "Out of range"; break;
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

DgError DgLogError_(const DgErrorCode error, const char * const path, const int line) {
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

/** Raise and guard **/

#ifndef MELON_MAX_GUARD_STACK_SIZE
	#define MELON_MAX_GUARD_STACK_SIZE 32
#endif

typedef struct {
	DgErrorGuardEntry entries[MELON_MAX_GUARD_STACK_SIZE];
	size_t top;
} DgErrorGuardArray;

DgErrorGuardArray gMelonErrorGuards;
DgErrorInfo gMelonCurrentError; // TODO: should be a stack to handle errors while handling errors

static inline DgErrorInfo *DgGetTopError_(void) {
	return &gMelonCurrentError;
}

DgErrorGuardEntry *DgGuardNextSlot_(void) {
	/**
	 * Return a pointer to the next available error guard entry, or raise an
	 * error if one is not available.
	 * 
	 * @todo Implement stack overflow check
	 */
	
	return &gMelonErrorGuards.entries[gMelonErrorGuards.top++];
}

DgErrorInfo *DgGuard_(int status) {
	/**
	 * If status is zero, then don't do much as we have already saved the
	 * enviornment, just return NULL. If it is nonzero that means we're handling
	 * a return from setjmp as an error, so return the error.
	 */
	
	if (status) {
		return DgGetTopError_();
	}
	else {
		return NULL;
	}
}

void DgUnguard(void) {
	/**
	 * Pop a guard entry off the stack
	 */
	
	gMelonErrorGuards.top--;
}

static void DgHandleFatalError_(DgErrorInfo *error) {
	/**
	 * Handle an error when there are no error guards left.
	 */
	
	DgLog(DG_LOG_FATAL, "An unhandled error was raised:\n"
		"     type: %s\n"
		"  message: %s\n"
		"     file: %s\n"
		" function: %s\n"
		"     line: %zu\n",
		error->type, error->message, error->file, error->function, error->line
	);
	
	abort();
}

void DgReraise(void) {
	/**
	 * Raise a currently set error
	 */
	
	// No error handlers on stack, so abort.
	if (gMelonErrorGuards.top == 0) {
		DgLog(DG_LOG_INFO, "top = %zu", gMelonErrorGuards.top);
		DgHandleFatalError_(&gMelonCurrentError);
	}
	else {
		DgErrorGuardEntry *entry = &gMelonErrorGuards.entries[--gMelonErrorGuards.top];
		longjmp(entry->env, 1);
	}
}

void DgRaise_(DgErrorInfo ei) {
	/**
	 * Raise a new error
	 */
	
	gMelonCurrentError = ei;
	DgReraise();
}

static void DgRaiseTest_somethingthatraisesanerror(void) {
	DgRaise("SomeError", "Some test error");
}

void DgRaise_Test(void) {
	DgTry({
		DgRaiseTest_somethingthatraisesanerror();
	}, error_info, {
		DgLog(DG_LOG_INFO, "Caught an error: %s", error_info->type);
	})
}
