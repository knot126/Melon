/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Compression
 * 
 * @note Currently some custom Huffman tree based compression
 */

#include "memory.h"
#include "memory_stream.h"
#include "bytes.h"
#include "string.h"
#include "log.h"

#include "compress.h"

size_t DgCompressRLE_CountBytesUntilConsecutiveWithMinimumWithMax(const uint8_t * const data, size_t length, size_t min_cons, size_t max) {
	/**
	 * Count the number of bytes until there is a consecutive seqence of a least
	 * min_cons + 1 bytes, of up to max bytes.
	 * 
	 * @param data Data
	 * @param length Length of data
	 * @param min_cons Minimum number of consecutive bytes to break the count
	 * @param max Max bytes to count
	 */
	
	size_t cons, i;
	
	for (i = 0; i < length;) {
		cons = DgStringCountConsecutiveWithMax(data + i, length - i, min_cons + 1);
		
		if (cons > min_cons || (i + cons) > max) {
			return i;
		}
		
		i += cons;
	}
	
	return i;
}

DgError DgCompressRLE(uint8_t *in_data, size_t in_size, uint8_t **out_data, size_t *out_size) {
	/**
	 * (Try to) compress the input using run-length coding
	 * 
	 * @param in_data Input data
	 * @param in_size Size of the input data
	 * @param out_data Pointer to where to store the pointer to the output
	 * @param out_size Pointer to where to store the output's size
	 * @return Any error while processing
	 */
	
	DgMemoryStream *output = DgMemoryStreamCreate();
	
	if (!output) {
		return DG_ERROR_ALLOCATION_FAILED;
	}
	
	// Kept out here for speed
	size_t cons;
	
	for (size_t i = 0; i < in_size;) {
		cons = DgStringCountConsecutiveWithMax(in_data + i, in_size - i, 0x80);
		
		if (cons > 2) {
			DgMemoryStreamWriteInt8(output, -cons);
			DgMemoryStreamWriteUInt8(output, in_data[i]);
			i += cons;
		}
		else if (cons == 0) {
			DgLog(DG_LOG_WARNING, "WTF condition: cons == 0");
		}
		else {
			cons = DgCompressRLE_CountBytesUntilConsecutiveWithMinimumWithMax(in_data + i, in_size - i, 2, 0x80);
			DgMemoryStreamWriteInt8(output, cons - 1);
			DgMemoryStreamWrite(output, cons, &in_data[i]);
			i += cons;
		}
	}
	
	DgBufferFromStream(output, (void **) out_data, out_size);
	
	return (*out_data) ? DG_ERROR_SUCCESS : DG_ERROR_FAILED;
}

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
