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
 * Fixed point numbers
 */ 

#include "fixed.h"

inline DgFixed DgFixedAdd(DgFixed a, DgFixed b) {
	/**
	 * Add two fixed point numbers
	 * 
	 * @param a First number
	 * @param b Second number
	 * @returns Result of a + b
	 */
	
	return a + b;
}

inline DgFixed DgFixedSub(DgFixed a, DgFixed b) {
	/**
	 * Subtract two fixed point numbers
	 * 
	 * @param a First number
	 * @param b Second number
	 * @returns Result of a - b
	 */
	
	return a - b;
}

inline DgFixed DgFixedFloor(DgFixed n) {
	/**
	 * Floors a fixed point number
	 * 
	 * @param n Number
	 * @returns Number floored
	 */
	
	return n & (~DG_FIXED_FRACTIONAL_BITS_MASK);
}

inline DgFixed DgFixedFrac(DgFixed n) {
	/**
	 * Floors the fractional part of a fixed point number
	 * 
	 * @param n Number
	 * @returns Fractional part of number
	 */
	
	return n & (DG_FIXED_FRACTIONAL_BITS_MASK);
}

inline DgFixed DgFixedMul(DgFixed a, DgFixed b) {
	/**
	 * Multiply two fixed point numbers
	 * 
	 * In order to fix some overflow, we can split one number into its fractional
	 * and whole parts, and then shift left on the whole part *before* the real
	 * multiplication, in order to increase percision of results. The fractional
	 * part can still stay the same.
	 * 
	 * C = (A * B) / base
	 * C = ((floor(A) + frac(A)) * B) / base
	 * C = (B * floor(A) + B * frac(A)) / base
	 * C = (B * (floor(A) * 1/base) + (B * frac(A)) * 1/base)
	 * 
	 * @todo It's also probably better to split B into frac/floor too.
	 * 
	 * @param a First number
	 * @param b Second number
	 * @returns Result of a * b
	 */
	
	return (b * (a >> DG_FIXED_FRACTIONAL_BITS)) + ((b * DgFixedFrac(a)) >> DG_FIXED_FRACTIONAL_BITS);
	
	//return (a * b) >> DG_FIXED_FRACTIONAL_BITS;
}

inline DgFixed DgFixedDiv(DgFixed a, DgFixed b) {
	/**
	 * Multiply two fixed point numbers
	 * 
	 * @todo Handle/fix overflow
	 * 
	 * @param a First number
	 * @param b Second number
	 * @returns Result of a / b
	 */
	
	return (a / b) << DG_FIXED_FRACTIONAL_BITS;
}
