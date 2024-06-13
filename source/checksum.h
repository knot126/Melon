/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * Hashing Strings and Data
 */ 

#pragma once

#include <inttypes.h>

uint32_t DgChecksumStringU32_DJB2(const char * str);
uint32_t DgChecksumU32_DJB2(size_t length, const char *data);
uint32_t DgChecksumStringU32(const char * str);
