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
#include "storage.h"
#include "storage_filesystem.h"
#include "file.h"

#include "compress.h"

static int8_t DgCompressRLE_AnalyseStream(const uint8_t *data, size_t length, size_t at) {
	/**
	 * Analyse the stream and return a verdict on the next best action.
	 * 
	 * If return n < 0: copy -n of the byte at `at` into the stream
	 * If return n >= 0: copy n + 1 bytes to the stream as is
	 */
	
	// Count the consecutive number of bytes for the current character
	size_t consec = DgStringCountConsecutiveWithMax(data + at, length - at, 0x80);
	
	DgLog(DG_LOG_VERBOSE, "consec = %llu", consec);
	
	if (consec > 2) {
		return -(consec & 0x7f);
	}
	
	// We didn't get a consecutive match, so count the number of bytes to raw
	// copy over. This goes until we do get a match or we reach the max number
	// of bytes in a copy operation.
	size_t i;
	
	for (i = 0; i < length; i++) {
		DgLog(DG_LOG_VERBOSE, "in loop: consec = %llu  i = %llu", consec, i);
		
		size_t consec = DgStringCountConsecutiveWithMax(data + at + i, length - at - i, 0x80);
		
		if (consec > 2 || (i + consec) > 0x80) {
			break;
		}
	}
	
	return (i - 1) & 0x7f;
}

DgError DgCompressRLE(const uint8_t *data, size_t length, uint8_t **output_data, size_t *output_length) {
	/**
	 * Compress a buffer to a bitwise RLE.
	 * 
	 * @param data
	 * @param length
	 * @param output_data
	 * @param output_length
	 * @return Error while compressing
	 */
	
	DgLog(DG_LOG_VERBOSE, "~~~ find output length ~~~");
	
	output_length[0] = 0;
	
	// Calculate the output length
	for (size_t i = 0; i < length;) {
		// Analyse the stream at this point
		int8_t asr = DgCompressRLE_AnalyseStream(data, length, i);
		
		DgLog(DG_LOG_VERBOSE, "-> asr = %d", asr);
		DgLog(DG_LOG_VERBOSE, "-> action = %s %d", (asr < 0) ? "expand" : "copy", (asr < 0) ? -asr : asr + 1);
		
		// >= 0 means we copy the next N + 1 bytes
		if (asr >= 0) {
			// asr + 1 is the number of bytes to copy + the command bit
			output_length[0] += (size_t) asr + 1 + 1;
			i += (size_t) asr + 1;
		}
		// < 0 means we expand the next -N bytes
		else {
			// only need a control bit and the bit to copy
			output_length[0] += 2;
			i += -asr;
		}
	}
	
	output_data[0] = DgMemoryAllocate(output_length[0]);
	
	if (output_data[0] == NULL) {
		return DG_ERROR_ALLOCATION_FAILED;
	}
	
	DgLog(DG_LOG_VERBOSE, "DgCompressRLE(): input is 0x%llx and output is 0x%llx bytes", length, output_length[0]);
	
	DgLog(DG_LOG_VERBOSE, "~~~ compute output ~~~");
	
	// i is input position, j is output position
	for (size_t i = 0, j = 0; i < length;) {
		// Analyse the stream at this point
		int8_t asr = DgCompressRLE_AnalyseStream(data, length, i);
		
		((int8_t **) output_data)[0][j] = asr;
		
		if (asr < 0) {
			output_data[0][j + 1] = data[i];
			j += 2;
			i += -asr;
		}
		else {
			// Copy the memory as is
			DgMemoryCopy((size_t) asr + 1, &data[i], &output_data[0][j + 1]);
			
			// (N + 1) bytes + command bit
			j += asr + 2;
			i += (size_t) asr + 1;
		}
	}
	
	return DG_ERROR_SUCCESS;
}

void DgCompressRLE_Test(void) {
	const char *sample1 = "BBBBBBABBAAAAAAAAABCDEFFFGGAGAGAABAAAAA";
	uint8_t *out;
	size_t outlen;
	
	DgError error = DgCompressRLE((uint8_t *) sample1, DgStringLength(sample1), &out, &outlen);
	
	if (error) {
		DgLogError(error);
	}
	
	DgStorageAddPool(NULL, DgFilesystemCreatePool("file", "."));
	
	DgFileSave(NULL, "file://test.rle", outlen, out);
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
