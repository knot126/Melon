/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Compression
 * 
 * @note Currently some custom Huffman tree based compression
 */

#pragma once

#include "common.h"

DgError DgCompressRLE(uint8_t *in_data, size_t in_size, uint8_t **out_data, size_t *out_size);
