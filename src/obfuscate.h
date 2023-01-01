/**
 * Copyright (C) 2021 - 2023 Knot126 and Descentix Software
 * ========================================================
 * 
 * Data Obfuscation (aka weak encryption)
 */

#pragma once

#include <inttypes.h>

void DgPolyalphabeticObfuscate(const char * restrict key, char static_shift, size_t size, uint8_t * restrict data);
void DgPolyalphabeticDeobfuscate(const char * restrict key, char static_shift, size_t size, uint8_t * restrict data);
