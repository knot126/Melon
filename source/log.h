/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * Logging Facialites
 */ 

#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <inttypes.h>

typedef int32_t DgLogLevel;

enum {
	DG_LOG_VERBOSE = -1,
	DG_LOG_SUCCESS = 0,
	DG_LOG_INFO = 1,
	DG_LOG_WARNING = 2,
	DG_LOG_DEPRECATION = 3,
	DG_LOG_ERROR = 4,
	DG_LOG_FATAL = 5,
};

void DgLogRaw(const char * const buffer);
void DgLog(const DgLogLevel level, const char * const format, ...);
void DgSetMinLogLevel(DgLogLevel level);
DgLogLevel DgGetMinLogLevel(void);
