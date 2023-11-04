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
 * Cryptographic hashing
 */

#include "error.h"
#include "alloc.h"

#include "crypto_hash.h"

#define ROTL(x, n) ((x << n) | (n >> (32 - n)))
#define SWAP(t, x, y) t temp = x; x = y; y = temp;

static void DgCryptoCubeHashRound(uint32_t * const block) {
	/**
	 * Preform one round of the core CubeHash function inplace
	 * 
	 * @see https://en.wikipedia.org/wiki/CubeHash
	 * @see https://cubehash.cr.yp.to/index.html
	 * 
	 * @param block Block to preform the round function on
	 */
	
	// 1. Add x[0jklm] into x[1jklm] modulo 2^32, for each (j,k,l,m). 
	for (size_t i = 0; i <= 0b1111; i++) {
		block[0b10000 | i] += block[i];
	}
	
	// 2. Rotate x[0jklm] upwards by 7 bits, for each (j,k,l,m). 
	for (size_t i = 0; i <= 0b1111; i++) {
		block[i] = ROTL(block[i], 7);
	}
	
	// 3. Swap x[00klm] with x[01klm], for each (k,l,m). 
	for (size_t i = 0; i <= 0b111; i++) {
		// uint32_t temp = block[i];
		// block[i] = block[0b1000 | i];
		// block[0b1000 | i] = temp;
		SWAP(uint32_t, block[i], block[0b1000 | i]);
	}
	
	// 4. Xor x[1jklm] into x[0jklm], for each (j,k,l,m). 
	for (size_t i = 0; i <= 0b1111; i++) {
		block[i] ^= block[0b10000 | i];
	}
	
	// 5. Swap x[1jk0m] with x[1jk1m], for each (j,k,m). 
	for (size_t i = 0; i <= 0b11; i++) {
		for (size_t j = 0; j <= 0b1; j++) {
			size_t k = (i << 2) | j;
			// uint32_t temp = block[0b10000 | k];
			// block[0b10000 | k] = block[0b10010 | k];
			// block[0b10010 | k] = temp;
			SWAP(uint32_t, block[0b10000 | k], block[0b10010 | k]);
		}
	}
	
	// 6. Add x[0jklm] into x[1jklm] modulo 2^32, for each (j,k,l,m). 
	for (size_t i = 0; i <= 0b1111; i++) {
		block[0b10000 | i] += block[i];
	}
	
	// 7. Rotate x[0jklm] upwards by 11 bits, for each (j,k,l,m). 
	for (size_t i = 0; i <= 0b1111; i++) {
		block[i] = ROTL(block[i], 11);
	}
	
	// 8. Swap x[0j0lm] with x[0j1lm], for each (j,l,m). 
	for (size_t i = 0; i <= 0b1; i++) {
		for (size_t j = 0; j <= 0b11; j++) {
			size_t k = (i << 3) | (j);
			// uint32_t temp = block[k];
			// block[k] = block[0b100 | k];
			// block[0b100 | k] = temp;
			SWAP(uint32_t, block[k], block[0b100 | k]);
		}
	}
	
	// 9. Xor x[1jklm] into x[0jklm], for each (j,k,l,m). 
	for (size_t i = 0; i <= 0b1111; i++) {
		block[i] ^= block[0b10000 | i];
	}
	
	// 10. Swap x[1jkl0] with x[1jkl1], for each (j,k,l). 
	for (size_t i = 0; i <= 0b111; i++) {
		size_t j = i << 1;
		// uint32_t temp = block[0b10000 | j];
		// block[0b10000 | j] = block[0b10001 | j];
		// block[0b10001 | j] = temp;
		SWAP(uint32_t, block[0b10000 | j], block[0b10001 | j]);
	}
}

#undef SWAP
#undef ROTL

DgError DgCryptoCubeHasherInit(DgCryptoCubeHasher *this, uint32_t i, uint32_t r, uint32_t b, uint32_t f, uint32_t h) {
	/**
	 * Initialise a streaming cube hasher and preform the first few rounds
	 * 
	 * @param this The streaming hasher
	 * @param i The number of rounds to preform on the initial input
	 * @param r The number of rounds per message block (higher = more secure)
	 * @param b The number of bytes per message block (lower = more secure)
	 * @param f The number of finishing rounds
	 * @param h The output length of the final hash
	 * @return Error, if there is any
	 */
	
	// Check for constraints
	if ((h % 8) != 0 || h > 512 || b > 128) {
		return DG_ERROR_OUT_OF_RANGE;
	}
	
	// Zero out states
	for (size_t i = 0; i < 32; i++) {
		this->state[i] = 0;
	}
	
	// Save parameters
	this->initial = i; // TODO: this one is not needed
	this->roundsperblock = r;
	this->bytesperblock = b;
	this->finishing = f;
	this->outputlen = h;
	
	// Configure the initial state
	this->state[0b00000] = h / 8;
	this->state[0b00001] = b;
	this->state[0b00010] = r;
	
	// Preform the initial i rounds
	for (size_t x = 0; x < i; x++) {
		DgCryptoCubeHashRound(&this->state);
	}
	
	return DG_ERROR_SUCCESS;
}

DgError DgCryptoCubeHasherNextBlock(DgCryptoCubeHasher *this, size_t length, uint8_t *block) {
	/**
	 * Process a message block no longer than this->bytesperblock, padding if
	 * that is needed.
	 * 
	 * @param this Streaming hasher instance
	 * @param length Length of input block
	 * @param block Input block data
	 * @return Error if any
	 */
	
	if (length > this->bytesperblock) {
		return DG_ERROR_OUT_OF_RANGE;
	}
	
	// Create the array to xor
	uint32_t to_xor[32] = { 0 };
	
	// Copy the input block into the state
	DgMemoryCopy(length, block, (void *) &to_xor);
	
	// Append padding if needed
	if (length < this->bytesperblock) {
		((uint8_t *)(&to_xor))[length] = 0x80;
	}
	
	// XOR the blocks
	for (size_t i = 0; i < 32; i++) {
		this->state[i] = to_xor[i];
	}
	
	// Transform the blocks
	for (size_t x = 0; x < this->roundsperblock; x++) {
		DgCryptoCubeHashRound(&this->state);
	}
	
	return DG_ERROR_SUCCESS;
}

DgError DgCryptoCubeHasherFinalise(DgCryptoCubeHasher *this, size_t * const length, uint8_t ** const hash) {
	/**
	 * Finalise the hash and output it
	 * 
	 * @warning If length and hash are not NULL, memory will be allocated which
	 * needs to be freed later.
	 * 
	 * @param this Streaming hasher instance
	 * @param length Out parameter for the length (in bytes) of the hash
	 * @param hash Out parameter for the heap-allocated memory of the hash
	 * @return Error if any
	 */
	
	// Xor 1 (I believe (?) this helps prevents length extension attacks)
	this->state[0b11111] ^= 1;
	
	// Preform the final f rounds
	for (size_t x = 0; x < this->finishing; x++) {
		DgCryptoCubeHashRound(&this->state);
	}
	
	// Output the hash (if possible)
	if (length && hash) {
		length[0] = this->outputlen / 8;
		hash[0] = DgMemoryAllocate(length[0]);
		DgMemoryCopy(length[0], &this->state, hash[0]);
	}
	
	return DG_ERROR_SUCCESS;
}
