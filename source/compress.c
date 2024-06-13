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

#include "alloc.h"
#include "string.h"
#include "log.h"

#include "compress.h"

typedef struct DgCompress_ByteProbabilityTable {
	size_t probs[0x100];
} DgCompress_ByteProbabilityTable;

static void DgCompress_CountByteProbability(DgCompress_ByteProbabilityTable *this, uint8_t *data, size_t length) {
	/**
	 * Count the probability of each byte in the given data
	 * 
	 * @param this Probability table
	 * @param length Length of the data
	 * @param data Data to analyse
	 */
	
	DgMemoryZero(this, sizeof *this);
	
	for (size_t i = 0; i < length; i++) {
		this->probs[data[i]] += 1;
	}
}
