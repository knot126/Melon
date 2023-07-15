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

uint32_t DgHashStringU32_DJB2(const char * str) {
	/**
	 * Hash a string using the DJB2 hash function. This is probably better than
	 * what I wrote above but maybe what I wrote above is faster by a little
	 * bit. (probably not) :)
	 * 
	 * The actual algorithm:
	 *     start with current = 5381
	 *     hashiter(current, next) = (current * 33) + next
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

uint32_t DgHashStringU32(const char * str) {
	/**
	 * Get a 32-bit hash of a string using the preferred fast algorithm.
	 * 
	 * @param str String to hash
	 * @return Hashed value
	 */
	
	return DgHashStringU32_DJB2(str);
}
