/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * Compression
 * 
 * @note Currently some custom Huffman tree based compression
 */

#pragma once

#include "common.h"

enum {
	DG_PROBABILITY_TREE_END = -1,
};

/**
 * The index where the item in the tree is what that item represents. For
 * example 'a' is at index 97 (since 'a' == 97).
 */
typedef struct DgCompressProbabilityTreeEntry {
	uint32_t occurances;
	int16_t next, prev;
} DgCompressProbabilityTreeEntry;

typedef struct DgCompressState {
	DgCompressProbabilityTreeEntry tree[256];
} DgCompressState;
