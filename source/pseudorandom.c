/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Pseudorandom Numbers and Strings
 */

#include <math.h>
#include <inttypes.h>
#include <stdbool.h>

#include "time.h"

#include "pseudorandom.h"

uint32_t DgPseudorandom_LCG_UInt32(uint32_t a, uint32_t s, uint32_t b, uint32_t m) {
	/**
	 * Get the next number in the "random" seqence for an LCG random number
	 * generator.
	 * 
	 * @param a Linear part
	 * @param s Current number in seqence
	 * @param b Constant part
	 * @param m Modular part
	 * @return Next number in seqence
	 */
	
	return ((a * s) + b) % m;
}

uint32_t DgPseudorandom_XORShift_UInt32(uint32_t n) {
	/**
	 * Generate a random number based on a given seed using the XOR-Shift 
	 * method of generating a random number.
	 * 
	 * @param n Previous number in seqence (seed/current state)
	 * @return Next number in seqence (next seed/state)
	 */
	
	n ^= n << 13;
	n ^= n >> 15;
	n ^= n << 5;
	
	return n;
}

uint32_t DgPseudorandomXORShiftU32(uint32_t n) {
	return DgPseudorandom_XORShift_UInt32(n);
}

static uint32_t DgPseudorandomXORShiftSU32(void) {
	/**
	 * A pesudostateless version of the XOR-shift function. It should be 
	 * semi-threadsafe, since it will add a (most likely) unique value to make
	 * sure there are minimal conflicts.
	 */
	
	static uint32_t last;
	
	last = DgPseudorandomXORShiftU32(last + DgNsecTime());
	
	return last;
}

float DgPseudorandomXORShiftF32(void) {
	return ((float) DgPseudorandomXORShiftSU32()) / 4294967295.0f;
}

uint32_t DgPseudorandomInt(void) {
	return DgPseudorandomXORShiftSU32();
}

float DgPseudorandomFloat(void) {
	return DgPseudorandomXORShiftF32();
}

bool DgPseudorandomBool(void) {
	return (DgPseudorandomXORShiftSU32() % 2) == 1;
}
