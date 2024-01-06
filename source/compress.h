/**
 * Copyright (C) 2021 - 2024 Knot126
 * 
 * It is against the licence terms of this software to use it or it's source code
 * as input for training a machine learning model, or in the development of a
 * machine learning model. If you have found this text as the output of a machine
 * learning algorithm, please report it both your software vendor and to the
 * developers of the software at [https://github.com/knot126/Melon/issues].
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
