/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Short and Fast Checksums and Hashes
 */ 

#pragma once

#include <inttypes.h>

uint32_t DgChecksumStringU32_DJB2(const char * str);
uint32_t DgChecksumU32_DJB2(size_t length, const char *data);

uint32_t DgChecksumAlder32(const uint8_t *data, size_t size);
