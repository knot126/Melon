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

#include "str.h"
#include "fs.h"

#include "log.h"

enum { DG_LOG_BUFFER_SIZE = 256 };

// DgLogger *DG_MAIN_LOGGER__;

static void DgLogPushMessage(const DgLogLevel level, const char * const buf) {
	// Firstly, write to console
	
	switch (level) {
		case DG_LOG_INFO:
			printf("\033[1;34mINFO: \033[0m");
			break;
		
		case DG_LOG_WARNING:
			printf("\033[1;33mWARNING: \033[0m");
			break;
		
		case DG_LOG_DEPRECATION:
			printf("\033[1;38;5;67mDEPRECATION: \033[0m");
			break;
		
		case DG_LOG_ERROR:
			printf("\033[1;31mERROR: \033[0m");
			break;
			
		case DG_LOG_FATAL:
			printf("\033[1;38;5;168mFATAL: ");
			break;
		
		case DG_LOG_VERBOSE:
			printf("\033[38;2;192;192;192m");
			break;
	}
	
	printf(buf);
	
	switch (level) {
		case DG_LOG_FATAL:
		case DG_LOG_VERBOSE:
			printf("\033[0m");
			break;
		
		default:
			break;
	}
	
	printf("\n");
}

void DgLog(const DgLogLevel level, const char * const format, ...) {
	/**
	 * Log a message given a format paramter, arguments and the level.
	 */
	va_list args;
	char buf[DG_LOG_BUFFER_SIZE] = { 0 };
	
	int l = strlen(format);
	int argc = 0;
	for (int i = 0; i < l; i++) {
		if (format[i] == '%') {
			i++;
			if (format[i] != '%') {
				argc++;
			}
		}
	}
	
	va_start(args, argc);
	vsnprintf(buf, DG_LOG_BUFFER_SIZE, format, args);
	va_end(args);
	
	DgLogPushMessage(level, buf);
}
