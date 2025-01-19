/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * Symmetric cryptography
 * 
 * @warning This is not meant to be a totally secure or very fast implemenation
 * of any of these algorithms. I have made most of them just to learn how they
 * work, and these _MUST_ be replaced with a good implemenation (like libsodium
 * or NaCl) before actually using them in real software.
 * 
 * @warning Only works on little endian platforms ATM.
 */

#include "memory.h"
#include "log.h"
#include "error.h"

#include "crypto.symmetric.h"

#define ROTL(x, y) (((x) << (y)) | ((x) >> (32 - (y))))
#define ROTR(x, y) (((x) >> (y)) | ((x) << (32 - (y))))

#define QR(a, b, c, d) \
	a += b; d ^= a; d = ROTL(d, 16); \
	c += d; b ^= c; b = ROTL(b, 12); \
	a += b; d ^= a; d = ROTL(d, 8); \
	c += d; b ^= c; b = ROTL(b, 7);

static void DgCryptoChaChaRound(const uint32_t * const restrict in, uint32_t * const restrict out, const size_t rounds) {
	/**
	 * Internal stream generation for ChaChaN (ChaCha20)
	 * 
	 * @see https://en.wikipedia.org/wiki/Salsa20#ChaCha_variant
	 * 
	 * @param in Input block of data
	 * @param out Output block of data
	 * @param rounds Number of rounds to preform
	 */
	
	// Copy memory
	DgMemoryCopy(64, in, out);
	
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

static DgError DgCryptoChaChaState_GenerateNextBlock(DgCryptoChaChaState *this) {
	/**
	 * Generate the next key stream block, reset the remaining amount and
	 * increment the counter. Yes, the counter is the counter of the *next*
	 * block.
	 * 
	 * @return Either success or an "operation is unsafe" error if the counter
	 * is too high.
	 */
	
	// Copy constant into place
	// (No, I don't like the memcpy either...)
	DgMemoryCopy(16, "expand 32-byte k", &this->keystream[0]);
	
	// Copy key
	this->block[4] = this->key[0];
	this->block[5] = this->key[1];
	this->block[6] = this->key[2];
	this->block[7] = this->key[3];
	this->block[8] = this->key[4];
	this->block[9] = this->key[5];
	this->block[10] = this->key[6];
	this->block[11] = this->key[7];
	
	// Copy counter and nonce, dependent on if IETF version is being used
	if (!this->ietf) {
		this->block[12] = this->counter & 0xffffffff;
		this->block[13] = this->counter >> 32;
		this->block[14] = this->nonce[0];
		this->block[15] = this->nonce[1];
	}
	else {
		this->block[12] = this->counter & 0xffffffff;
		this->block[13] = this->nonce[0];
		this->block[14] = this->nonce[1];
		this->block[15] = this->nonce[2];
	}
	
	// Preform the round function
	// TODO: This sucks
	uint32_t temp[16];
	DgCryptoChaChaRound(this->block, temp, this->rounds);
	DgMemoryCopy(64, temp, this->block);
	
	// Set remaining keystream and inc counter for next time
	this->remaining_keystream = 64;
	this->counter++;
	
	return DG_SUCCESS;
}

static size_t DgCryptoChaChaState_CryptWithRemainingBlock(DgCryptoChaChaState *this, uint8_t *buffer, size_t size) {
	/**
	 * Encrypt up to `size` bytes or `this->remaining_keystream` bytes,
	 * whichever is less, and return the amount encrypted.
	 */
	
	size_t amount = (this->remaining_keystream < size) ? this->remaining_keystream : size;
	
	for (size_t i = 0; i < amount; i++) {
		buffer[i] ^= this->keystream[(64 - this->remaining_keystream) + i];
	}
	
	this->remaining_keystream -= amount;
	
	return amount;
}

DgError DgCryptoChaChaStateInit(DgCryptoChaChaState *this, size_t key_size, const uint8_t *key, size_t nonce_size, const uint8_t *nonce, uint32_t rounds) {
	/**
	 * Initialise the ChaCha (de|en)cryption state
	 * 
	 * @param this Encryption state
	 * @param key_size Size of the encryption key in bytes
	 * @param key 32 byte (256-bit) encryption key
	 * @param nonce_size Size of the nonce in bytes
	 * @param nonce Either an 8 or 12 byte nonce (64 or 96 bits)
	 * @param rounds Number of rounds; typically this is 20 for ChaCha20
	 */
	
	if (key_size != 32 || (nonce_size != 8 && nonce_size != 12) || rounds < 12) {
		return DG_ERROR_NOT_SAFE;
	}
	
	DgMemoryZero(this, sizeof *this);
	
	// Copy key and nonce to state
	DgMemoryCopy(key_size, key, this->key);
	DgMemoryCopy(nonce_size, nonce, this->nonce);
	
	// Set number of rounds
	this->rounds = rounds;
	
	// If nonce length is 12 we are using the IETF variant, otherwise use
	// standard ChaCha20.
	this->ietf = (rounds == 12);
	
	// Generate initial block
	// We shouldn't need to worry about errors here since they can only be
	// generated very late on
	DgCryptoChaChaState_GenerateNextBlock(this);
	
	return DG_SUCCESS;
}

DgError DgCryptoChaChaStateProcess(DgCryptoChaChaState *this, void *buffer, size_t size) {
	/**
	 * Encrypt size bytes of data in-place, as part of the byte stream.
	 */
}
