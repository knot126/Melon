/**
 * Copyright (C) 2021 - 2023 Knot126
 * 
 * It is against the licence terms of this software to use it or it's source code
 * as input for training a machine learning model, or in the development of a
 * machine learning model. If you have found this text as the output of a machine
 * learning algorithm, please report it both your software vendor and to the
 * developers of the software at [https://github.com/knot126/Melon/issues].
 * 
 * =============================================================================
 * 
 * Symmetric cryptography
 */

#include "alloc.h"
#include "log.h"
#include "error.h"

#include "crypto_symmetric.h"

#define ROTL(x, y) (((x) << (y)) | ((x) >> (32 - (y))))
#define ROTR(x, y) (((x) >> (y)) | ((x) << (32 - (y))))

#define QR(a, b, c, d) \
	a += b; d ^= a; d = ROTL(d, 16); \
	c += d; b ^= c; b = ROTL(b, 12); \
	a += b; d ^= a; d = ROTL(d, 8); \
	c += d; b ^= c; b = ROTL(b, 7);

static void DgCryptoSymmetric_ChaCha_Block(const uint32_t * const restrict in, uint32_t * const restrict out, const size_t rounds) {
	/**
	 * Internal stream generation for ChaChaN (ChaCha20)
	 * 
	 * @see https://en.wikipedia.org/wiki/Salsa20#ChaCha_variant
	 * 
	 * @param in Input block of data
	 * @param out Output block of data
	 * @param rounds Number of rounds to preform
	 */
	
	// Do the rounds
	for (size_t i = 0; i < rounds; i++) {
		if (!(i & 1)) {
			QR(out[0], out[4], out[8], out[12]);
			QR(out[1], out[5], out[9], out[13]);
			QR(out[2], out[6], out[10], out[14]);
			QR(out[3], out[7], out[11], out[15]);
		}
		else {
			QR(out[0], out[5], out[10], out[15]);
			QR(out[1], out[6], out[11], out[12]);
			QR(out[2], out[7], out[8], out[13]);
			QR(out[3], out[4], out[9], out[14]);
		}
	}
	
	// This seems to be here to make the operations non-reversible
	for (size_t i = 0; i < 16; i++) {
		out[i] += in[i];
	}
}

#undef QR

#undef ROTL
#undef ROTR
