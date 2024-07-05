/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
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
		uint16_t a;
		uint8_t b;
	} test;
	
	test.a = 0xbeef;
	
	bool is_little = (test.b == 0xef);
	
	return is_little ? DG_ENDIAN_LITTLE : DG_ENDIAN_BIG;
}
