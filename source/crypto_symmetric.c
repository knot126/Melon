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
	a += b; d ^= a; ROTL(d, 16); \
	c += d; b ^= c; ROTL(b, 12); \
	a += b; d ^= a; ROTL(d, 8); \
	c += d; b ^= c; ROTL(b, 7);

static void DgCryptoSymmetric_ChaCha20_Block(const uint32_t * const restrict in, uint32_t * const restrict out, const size_t rounds) {
	/**
	 * Internal stream generation for ChaCha20
	 * 
	 * @see https://en.wikipedia.org/wiki/Salsa20#ChaCha_variant
	 * 
	 * @param rounds number of rounds of ChaCha20, secure = 20, fast = 12
	 * @param key key to use, 256 bits / 32 bytes
	 * @param counter counter, 64 bits / 8 bytes, can either go up one per block or byte afaik
	 * @param nonce random nonce, 64 bits / 8 bytes, should be actually random
	 * @param out output keystream block, 16 unsigned 32-bit words
	 */
	
	// Copy the shit into place for ChaCha
	// DgMemoryCopy(16, "expand 32-byte k", &out[0]);
	// DgMemoryCopy(32, key, &out[4]);
	// DgMemoryCopy(8, counter, &out[12]);
	// DgMemoryCopy(8, nonce, &out[14]);
	
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
