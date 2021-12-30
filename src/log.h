/*
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
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
