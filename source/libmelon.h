/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Melon library info and tools
 */

#pragma once

#include "common.h"
#include "error.h"

#define DG_MELON_VERSION(y, m, d) (y << 9) | (m << 5) | (d)

typedef uint32_t DgMelonVersion;

DgMelonVersion DgMelonGetVersion(void);
DgError DgMelonInit(void);
void DgMelonFree(void);
