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

#include "fs.h"

typedef enum DgLogLevel {
	DG_LOG_VERBOSE = -1,
	DG_LOG_INFO = 0,
	DG_LOG_WARNING = 1,
	DG_LOG_DEPRECATION = 2,
	DG_LOG_ERROR = 3,
	DG_LOG_FATAL = 4,
} DgLogLevel;

typedef struct DgLogger {
	DgFile *log_file;
} DgLogger;

void DgLog(const DgLogLevel level, const char * const format, ...);
