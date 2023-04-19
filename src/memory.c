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
 * Memory and bit-level access
 */

#include "memory.h"

bool DgBitRead(void *base, size_t bit) {
	/**
	 * Read a bit at the given memory address.
	 * 
	 * @param base Byte-level address where the bit is
	 * @param bit The index of the bit to read, where 0 is the leftmost and 7 is
	 * the rightmost.
	 * @return Value of the bit
	 */
	
	// Find the proper base
	base += (bit / 8);
	bit = bit % 8;
	
	// Read the bit
	return ((((uint8_t *) base)[0]) >> (7 - bit)) & 1;
}

uint64_t DgBitsRead(void *base, size_t bits, size_t count) {
	/**
	 * Read up to 64 bits.
	 * 
	 * @param base Base byte where the bits start
	 * @param bits Bit-level offset to start at
	 * @param 
	 */
	
	uint64_t result = 0;
	
	// Limit amount of bits
	count %= (sizeof result * 8);
	
	// Read in bits
	for (size_t i = 0; i < count; i++) {
		result |= DgBitRead(base, bits + i) << (((sizeof result * 8) - 1) - i);
	}
	
	// Adjust value to match count of bits read
	result = result >> ((sizeof result * 8) - count);
	
	return result;
}
