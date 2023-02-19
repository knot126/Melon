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
 * Common types and global variables
 */

#pragma once

#if defined(DG_MELON_TYPES)
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
#endif
