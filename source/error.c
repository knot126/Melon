/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Error handling
 */

#include <stdbool.h>
#include <setjmp.h>
#include "log.h"

// Linux gets backtraces
#ifdef __linux__
	#include <stdio.h>
	#include <execinfo.h>
	#define DG_MELON_MAX_BACKTRACE_COUNT 20
#endif

#include "error.h"

/** Error codes **/

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
bool gMelonEnableRaise = true;

static inline DgErrorInfo *DgGetTopError_(void) {
	return &gMelonCurrentError;
}

DgErrorGuardEntry *DgGuardNextSlot_(void) {
	/**
	 * Return a pointer to the next available error guard entry, or raise an
	 * error if one is not available.
	 * 
	 * @throws GuardStackOverflow if the guard stack is at max capacity
	 */
	
	if (gMelonErrorGuards.top >= MELON_MAX_GUARD_STACK_SIZE) {
		DgRaise("GuardStackOverflow", "The guard stack execeeded its maxium size");
	}
	
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
	
	// Some ugly code to print a backtrace on linux
#ifdef __linux__
	void *return_address[DG_MELON_MAX_BACKTRACE_COUNT];
	int len = backtrace(return_address, DG_MELON_MAX_BACKTRACE_COUNT);
	// We don't call from a signal handler so malloc() shouldn't be a problem
	char **backtrace = backtrace_symbols(return_address, len);
	
	if (backtrace) {
		DgLog(DG_LOG_VERBOSE, "backtrace:");
		for (int i = 0; i < len; i++) {
			DgLog(DG_LOG_VERBOSE, "    %s", backtrace[i]);
		}
		free(backtrace);
	}
#endif
	
	abort();
}

void DgReraise(void) {
	/**
	 * Raise a currently set error
	 */
	
	// No error handlers on stack, so abort.
	if (gMelonErrorGuards.top == 0) {
		DgHandleFatalError_(&gMelonCurrentError);
	}
	// Otherwise we pass control back to the last guard
	else {
		DgErrorGuardEntry *entry = &gMelonErrorGuards.entries[--gMelonErrorGuards.top];
		longjmp(entry->env, 1);
	}
}

void DgRaise_(DgErrorInfo ei) {
	/**
	 * Raise a new error
	 */
	
	// If raise is not enabled then don't do anything
	if (!gMelonEnableRaise) { return; }
	
	// Set current error
	gMelonCurrentError = ei;
	
	// Raise the current error
	DgReraise();
}

void DgErrorSetRaiseEnabled(bool enabled) {
	/**
	 * Set if raise operations should be enabled or disabled.
	 */
	
	gMelonEnableRaise = enabled;
}

bool DgErrorGetRaiseEnabled(void) {
	/**
	 * Get if DgRaise operations will work as expected or if they will be
	 * ignored.
	 */
	
	return gMelonEnableRaise;
}

static void DgRaiseTest_Func1() {
	DgLog(DG_LOG_INFO, "Func 1!");
}

static int DgRaiseTest_Func2(int a, int b) {
	DgLog(DG_LOG_INFO, "Func 2!");
	DgRaise("SomeError", "Some error raised from func 2");
	return 3;
}

static void DgRaiseTest_Func3(int c) {
	DgLog(DG_LOG_INFO, "Func 3!");
}

void DgRaise_Test(void) {
	// protected call test
	DgTry({
		DgRaiseTest_Func1();
		int a = DgRaiseTest_Func2(2, 5);
		DgRaiseTest_Func3(a);
	}, error_info, {
		DgLog(DG_LOG_INFO, "Caught an error: %s: %s", error_info->type, error_info->message);
	})
	
	// unprotected call test
	// DgRaiseTest_Func2(2, 5);
}

