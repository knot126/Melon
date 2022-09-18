/**
 * Copyright (C) 2021 - 2022 Knot126
 * =================================
 * 
 * Bitmaps and Other Images
 */ 

#pragma once

#include <inttypes.h>

/**
 * I think 2^13 is a nice base, it has a decent step size (1/8192) and can still
 * represent fairly large values (up to 2 ^ (31 - 13) = 262144) but of course I
 * have made it easy to change :)
 */
enum {
	DG_FIXED_FRACTIONAL_BITS = 13,
	DG_FIXED_FRACTIONAL_BITS_MASK = 0x1fff,
};

typedef int32_t DgFixed;

DgFixed DgFixedAdd(DgFixed a, DgFixed b);
DgFixed DgFixedSub(DgFixed a, DgFixed b);
DgFixed DgFixedMul(DgFixed a, DgFixed b);
DgFixed DgFixedDiv(DgFixed a, DgFixed b);

DgFixed DgFixedFloor(DgFixed n);
DgFixed DgFixedFrac(DgFixed n);
