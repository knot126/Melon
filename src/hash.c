/*
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * Hashing Strings and Data
 */ 

#include <inttypes.h>

uint32_t DgHashStringU32_366(const char * str) {
	/**
	 * Hash a string using a custom algorithm, derived from DJB2. From some 
	 * light testing (1000 data points - so not that much) this seems to work
	 * quite well and removes an operation.
	 * 
	 * In practise, I think this probably starts to fail with a larger data
	 * table or when the strings are very similar. DJB2's magic number is said
	 * to be the best and while I cannot find why exactly that is, I will just
	 * go with the assumption that it is probably better in some way.
	 * 
	 * A possibly less ambiguous implementation that should do the same thing
	 * (aka a more human-readable description of what happens here):
	 *     start with current = 0x66666666
	 *     hashiter(current, next) = (current * 8) + next
	 * 
	 * I'm just calling this the 366 hashing algorithm since we use the
	 * constants 3 and 0x66666666.
	 */
	
	uint32_t hash = 0x66666666, next;
	
	while ((next = *str++)) {
		hash = (hash << 3) + next;
	}
	
	return hash;
}

uint32_t DgHashStringU32_DJB2(const char * str) {
	/**
	 * Hash a string using the DJB2 hash function. This is probably better than
	 * what I wrote above but maybe what I wrote above is faster by a little
	 * bit. (probably not) :)
	 * 
	 * The actual algorithm:
	 *     start with current = 5381
	 *     hashiter(current, next) = (current * 33) + next
	 */
	
	uint32_t hash = 5381, next;
	
	while ((next = *str++)) {
		hash = ((hash << 5) + hash) + next;
	}
	
	return hash;
}

uint32_t DgHashStringU32(const char * str) {
	/**
	 * Hash a string using the preferred fast algorithm.
	 */
	
	return DgHashStringU32_366(str);
}
