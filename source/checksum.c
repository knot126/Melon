/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Short and Fast Checksums and Hashes
 */ 

#include "common.h"

#include "checksum.h"

uint32_t DgChecksumStringU32_DJB2(const char * str) {
	/**
	 * Hash a string using the DJB2 hash function. This is probably better than
	 * what I wrote above but maybe what I wrote above is faster by a little
	 * bit. (probably not) :)
	 * 
	 * The actual algorithm:
	 *     start with current = 5381
	 *     hashiter(current, next) = (current * 33) + next
	 * 
	 * @see http://www.cse.yorku.ca/~oz/hash.html
	 * 
	 * @param str String to hash
	 * @return Hash value
	 */
	
	uint32_t hash = 5381, next;
	
	while ((next = *str++)) {
		hash = ((hash << 5) + hash) + next;
	}
	
	return hash;
}

uint32_t DgChecksumU32_DJB2(size_t length, const char *data) {
	/**
	 * Compute the 32-bit DJB2 checksum of the `length`-byte `data` value.
	 * 
	 * @param length Length of the data to hash
	 * @param data Data to hash
	 */
	
	uint32_t hash = 5381;
	size_t i = length;
	
	while (i--) {
		hash = ((hash << 5) + hash) + data[i];
	}
	
	return hash;
}

uint32_t DgChecksumAlder32(const uint8_t *data, size_t size) {
	/**
	 * Compute the Alder32 checksum of a data buffer
	 */
	
	uint16_t a = 1, b = 0;
	
	for (size_t i = 0; i < size; i++) {
		a += data[i];
		b += a;
	}
	
	return ((uint32_t)(b % 65521) << 16) | (a % 65521);
}
