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
 * Pseudorandom Numbers and Strings
 */

#pragma once

#include <inttypes.h>
#include <stdbool.h>

uint32_t DgPseudorandomXORShiftU32(uint32_t n);
float DgPseudorandomXORShiftF32(void);

uint32_t DgPseudorandomInt(void);
float DgPseudorandomFloat(void);
bool DgPseudorandomBool(void);
