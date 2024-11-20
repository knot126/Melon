/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * Functions to query about the architecture of the running machine
 */

#pragma once

#include "common.h"

enum {
	DG_ENDIAN_BIG = 0,
	DG_ENDIAN_LITTLE = 1,
};

bool DgMachineEndian(void);
