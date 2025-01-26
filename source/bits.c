/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Memory and bit-level access
 */

#include "bits.h"

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
	 * @note The bits will be read in the order:
	 * 
	 * ```
	 * 01234567 01234567 01234567 01234567
	 *    ^^^^^ ^^^^^^^^ ^^^^               is read into the int as:
	 * xxxxxxxx xxxxxxx3 45670123 45670123
	 * ```
	 * 
	 * @param base Base byte where the bits start
	 * @param bits Bit-level offset to start at
	 * @param count Number of bits to read, up to 64
	 * @return The read bits, as if it were a `count`-bit integer cast to a u64
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
