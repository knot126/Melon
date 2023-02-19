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
 * Random Numbers and Strings
 */

#include <math.h>
#include <inttypes.h>
#include <stdbool.h>

#include "time.h"

#include "rand.h"

#if 1
	#define extra
#else
	#define extra static
#endif

extra uint32_t DgRandXORShiftU32(uint32_t n) {
	/**
	 * Generate a random number based on a given seed using the XOR-Shift 
	 * method of generating a random number.
	 */
	
	n ^= n << 13;
	n ^= n >> 15;
	n ^= n << 5;
	
	return n;
}

static uint32_t DgRandXORShiftSU32(void) {
	/**
	 * A pesudostateless version of the XOR-shift function. It should be 
	 * semi-threadsafe, since it will add a (most likely) unique value to make
	 * sure there are minimal conflicts.
	 */
	
	static uint32_t last;
	
	last = DgRandXORShiftU32(last + DgNsecTime());
	
	return last;
}

extra float DgRandXORShiftF32(void) {
	return ((float) DgRandXORShiftSU32()) / 4294967295.0f;
}

uint32_t DgRandInt(void) {
	return DgRandXORShiftSU32();
}

float DgRandFloat(void) {
	return DgRandXORShiftF32();
}

bool DgRandBool(void) {
	return (DgRandXORShiftSU32() % 2) == 1;
}

#undef extra
