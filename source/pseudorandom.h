/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
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

// Shorthands
#define DgPrandInt DgPseudorandomInt
#define DgPrandFloat DgPseudorandomFloat
#define DgPrandBool DgPseudorandomBool
