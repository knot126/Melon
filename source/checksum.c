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
 * Hashing Strings and Data
 */ 

#include <inttypes.h>

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

uint32_t DgChecksumStringU32(const char * str) {
	/**
	 * Get a 32-bit hash of a string using the preferred fast algorithm.
	 * 
	 * @deprecated Any new code should use DgStringSeminise for hashing. If you
	 * need to use the specific hashing algorithm that was used for this
	 * function, use DgChecksumStringU32_DJB2.
	 * 
	 * @param str String to hash
	 * @return Hashed value
	 */
	
	return DgChecksumStringU32_DJB2(str);
}
