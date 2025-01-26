/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Functions to query about the architecture of the running machine
 */

#include "log.h"
#include "machine.h"

bool DgMachineEndian(void) {
	/**
	 * Return the endian of the machine
	 * 
	 * @return DG_ENDIAN_BIG if the machine is big endian, DG_ENDIAN_LITTLE if
	 * it is little.
	 */
	
	// TODO: This really should be tested better.
	
	union {
		uint16_t set_to_one;
		uint8_t is_little;
	} test;
	
	test.set_to_one = 1;
	
	return test.is_little;
}
