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
 * Logging Facialites
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "string.h"

#include "log.h"

enum { DG_LOG_BUFFER_SIZE = 1024 };

static DgLogLevel gMinLogLevel = DG_LOG_VERBOSE;

void DgLogRaw(const char * const buffer) {
	/**
	 * Write the given raw C string to the console.
	 * 
	 * @param buffer Buffer to write
	 */
	
	fprintf(stderr, "%s", buffer);
}

static void DgLogPushMessage(const DgLogLevel level, const char * const buf) {
	/**
	 * Prints out a message to the console.
	 * 
	 * @param level Level string that should be used
	 * @param buf String to print out
	 */
	
	const char *prefix = "";
	
	// Find the right prefix
	switch (level) {
		case DG_LOG_SUCCESS: prefix = "\033[1;32mSUCCESS: \033[0m"; break;
		case DG_LOG_INFO: prefix = "\033[1;34mINFO: \033[0m"; break;
		case DG_LOG_WARNING: prefix = "\033[1;33mWARNING: \033[0m"; break;
		case DG_LOG_DEPRECATION: prefix = "\033[1;38;5;67mDEPRECATION: \033[0m"; break;
		case DG_LOG_ERROR: prefix = "\033[1;31mERROR: \033[0m"; break;
		case DG_LOG_FATAL: prefix = "\033[1;38;5;168mFATAL: "; break;
		case DG_LOG_VERBOSE: prefix = "\033[38;2;192;192;192m"; break;
	}
	
	// Print prefix and then error message
	DgLogRaw(prefix);
	DgLogRaw(buf);
	
	switch (level) {
		case DG_LOG_FATAL:
		case DG_LOG_VERBOSE:
			DgLogRaw("\033[0m");
			break;
		
		default:
			break;
	}
	
	DgLogRaw("\n");
}

void DgLog(const DgLogLevel level, const char * const format, ...) {
	/**
	 * Put a message into the log given the log level, base string and items
	 * to format. This function uses C-style string formatting.
	 * 
	 * @param level Level to log at
	 * @param format Basic format string, the same as in printf()
	 * @param ... Format params
	 */
	
	// Check minium log level
	if (level < gMinLogLevel) {
		return;
	}
	
	va_list args;
	char buf[DG_LOG_BUFFER_SIZE] = { 0 };
	
	int l = strlen(format);
	int argc = 0;
	
	// Count number of escaped parts
	for (int i = 0; i < l; i++) {
		if (format[i] == '%') {
			i++;
			
			if (format[i] != '%') {
				argc++;
			}
		}
	}
	
	// Do formatting
	va_start(args, argc);
	vsnprintf(buf, DG_LOG_BUFFER_SIZE, format, args);
	va_end(args);
	
	// Print message to console
	DgLogPushMessage(level, buf);
}

void DgSetMinLogLevel(DgLogLevel level) {
	/**
	 * Set the minimum log level for logs to show in the console.
	 * 
	 * @param level Minimum level for logs to show at
	 */
	
	gMinLogLevel = level;
}

DgLogLevel DgGetMinLogLevel(void) {
	/**
	 * Get the current minimum log level.
	 * 
	 * @return Current log level
	 */
	
	return gMinLogLevel;
}
