/*
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * Common types and global variables
 */

#pragma once

#if defined(DG_TRESTLE_USE_UINT8_AS_BYTE) || defined(DG_TRESTLE_USE_FIXEDPOINT)
	#include <inttypes.h>
#endif

#ifdef DG_TRESTLE_USE_UINT8_AS_BYTE
	typedef uint8_t byte_t;
	typedef uint8_t byte;
#else
	typedef unsigned char byte_t;
	typedef unsigned char byte;
#endif

typedef long double real;

#ifdef DG_TRESTLE_USE_FIXEDPOINT
	typedef int64_t fixed64_t;
	#define DG_TRESTLE_FIXED64_FACTOR (1<<32)
	
	typedef int32_t fixed32_t;
	#define DG_TRESTLE_FIXED32_FACTOR (1<<16)
	
	typedef int16_t fixed16_t;
	#define DG_TRESTLE_FIXED16_FACTOR (1<<8)
#endif
