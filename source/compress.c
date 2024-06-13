/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * Compression
 * 
 * @note Currently some custom Huffman tree based compression
 */

#include "memory.h"
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
