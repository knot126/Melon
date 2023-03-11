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
 * Symmetric encryption ciphers
 */ 

#include "log.h"
#include "error.h"

#include "encrypt.h"

#define ROTL(x, y) (((x) << (y)) | ((x) >> (32 - (y))))
#define ROTR(x, y) (((x) >> (y)) | ((x) << (32 - (y))))

DgError DgSalsa20BlockEncryptInit(DgSalsa20Block *this, uint32_t *key, uint32_t *nonce, uint32_t *position) {
	/**
	 * Initialise the internal state of a Salsa20 block used for encryption
	 * 
	 * @param this State object
	 * @param key The encryption key
	 * @param nonce The cryptographic nonce
	 * @param pos The initial position
	 * @return Error code
	 */
	
	// Constants
	this->asU8[0  * 4 + 0] = 'e';
	this->asU8[0  * 4 + 1] = 'x';
	this->asU8[0  * 4 + 2] = 'p';
	this->asU8[0  * 4 + 3] = 'a';
	this->asU8[5  * 4 + 0] = 'n';
	this->asU8[5  * 4 + 1] = 'd';
	this->asU8[5  * 4 + 2] = ' ';
	this->asU8[5  * 4 + 3] = '3';
	this->asU8[10 * 4 + 0] = '2';
	this->asU8[10 * 4 + 1] = '-';
	this->asU8[10 * 4 + 2] = 'b';
	this->asU8[10 * 4 + 3] = 'y';
	this->asU8[15 * 4 + 0] = 't';
	this->asU8[15 * 4 + 1] = 'e';
	this->asU8[15 * 4 + 2] = ' ';
	this->asU8[15 * 4 + 3] = 'k';
	
	// The key
	this->asU32[1] = key[0];
	this->asU32[2] = key[1];
	this->asU32[3] = key[2];
	this->asU32[4] = key[3];
	this->asU32[11] = key[4];
	this->asU32[12] = key[5];
	this->asU32[13] = key[6];
	this->asU32[14] = key[7];
	
	// Nonce
	this->asU32[6] = nonce[0];
	this->asU32[7] = nonce[1];
	
	// Position
	this->asU32[8] = position[0];
	this->asU32[9] = position[1];
	
	return DG_ERROR_SUCCESSFUL;
}

#define S20_QUARTER_ROUND(a, b, c, d) \
	b ^= ROTL(a + d, 7); \
	c ^= ROTL(b + a, 9); \
	d ^= ROTL(c + b, 13); \
	a ^= ROTL(d + c, 18);

#define S20_GRID(c, r) temp.asU32[(c * 4) + r]

DgError DgSalsa20BlockHash(const DgSalsa20Block *in, DgSalsa20Block *out) {
	/**
	 * Hash the input block to the output block
	 * 
	 * @param in The input block
	 * @param out The output block
	 * @return Error code
	 */
	
	DgSalsa20Block temp;
	
	// Copy state to temp
	temp = in[0];
	
	// Do the 20 rounds
	for (uint16_t round = 0; round < 20; round += 2) {
		// Odd round - column major, shifting left one in the row each time
		S20_QUARTER_ROUND(S20_GRID(0, 0), S20_GRID(1, 0), S20_GRID(2, 0), S20_GRID(3, 0));
		S20_QUARTER_ROUND(S20_GRID(1, 1), S20_GRID(2, 1), S20_GRID(3, 1), S20_GRID(0, 1));
		S20_QUARTER_ROUND(S20_GRID(2, 2), S20_GRID(3, 2), S20_GRID(0, 2), S20_GRID(1, 2));
		S20_QUARTER_ROUND(S20_GRID(3, 3), S20_GRID(0, 3), S20_GRID(1, 3), S20_GRID(2, 3));
		
		// Even round - row major, shifting one column left each time
		S20_QUARTER_ROUND(S20_GRID(0, 0), S20_GRID(0, 1), S20_GRID(0, 2), S20_GRID(0, 3));
		S20_QUARTER_ROUND(S20_GRID(1, 1), S20_GRID(1, 2), S20_GRID(1, 3), S20_GRID(1, 0));
		S20_QUARTER_ROUND(S20_GRID(2, 2), S20_GRID(2, 3), S20_GRID(2, 0), S20_GRID(2, 1));
		S20_QUARTER_ROUND(S20_GRID(3, 3), S20_GRID(3, 0), S20_GRID(3, 1), S20_GRID(3, 2));
	}
	
	// Copy to output
	out[0] = temp;
	
	// Add in to out
	for (size_t i = 0; i < 16; i++) {
		out->asU32[i] += in[i];
	}
	
	return DG_ERROR_SUCCESSFUL;
}

#undef S20_QUARTER_ROUND
#undef S20_GRID

DgError DgSalsa20BlockEncrypt(size_t key_length, const uint8_t *key, size_t data_length, uint8_t *data) {
	/**
	 * Encrypt data using Salsa20
	 */
	
	DgSalsa20BlockHash a, b;
	
	DgSalsa20BlockEncryptInit(&a, key);
}

#undef ROTL
#undef ROTR
