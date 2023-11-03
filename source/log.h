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
