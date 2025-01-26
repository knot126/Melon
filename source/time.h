/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Time Functions
 */

#pragma once

#include <inttypes.h>

void DgInitTime(void);

double DgTime(void);
double DgRealTime(void);

uint32_t DgNsecTime(void);

void DgSleep(double length);
