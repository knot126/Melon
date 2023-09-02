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
 * Data Obfuscation
 * 
 * @note These are really just weak ciphers. They are only to be used for hinding
 * things from non-technical people; e.g. hiding the yiff folder from grandma.
 * Your local intellegence service might be able to crack this if they like.
 */

#include "string.h"
#include "pseudorandom.h"

#include "obfuscate.h"

#ifdef DG_MELON_OBFUSCATE_EXTRA
#define DG_POLYALPHABETIC_SHIFT() (key[i % key_length] + static_shift)

void DgObfuscate_Rot13(size_t length, uint8_t * restrict data) {
	/**
	 * Obfuscate data (only alphabetic chars) using the ROT13 cipher.
	 * 
	 * @param length Length of the data
	 * @param data Data to obfuscate
	 */
	
	for (size_t i = 0; i < length; i++) {
		char c = data[i];
		
		// Encrypt lowercase
		if (c >= 'a' && c <= 'z') {
			c = (((c - 'a') + 13) % 26) + 'a';
		}
		
		// Encrypt uppercase
		if (c >= 'A' && c <= 'Z') {
			c = (((c - 'A') + 13) % 26) + 'A';
		}
		
		data[i] = c;
	}
}

void DgObfuscate_Polyalphabetic(const char * restrict key, char static_shift, size_t size, uint8_t * restrict data) {
	/**
	 * Obfuscate data using a polyalphabetic cipher. You can also have an extra
	 * static shift character.
	 * 
	 * @param key The key that should be used to obfuscate the data
	 * @param static_shift A static shift character (can just be zero)
	 * @param size Size of the data
	 * @param data Data buffer to encrypt
	 */
	
	size_t key_length = DgStringLength(key);
	
	// If the key length is zero or NULL just use the shift character
	if (!key_length) {
		for (size_t i = 0; i < size; i++) {
			data[i] += static_shift;
		}
		
		return;
	}
	
	// Otherwise, do the standard shift
	for (size_t i = 0; i < size; i++) {
		data[i] += DG_POLYALPHABETIC_SHIFT();
	}
}

void DgDeobfuscate_Polyalphabetic(const char * restrict key, char static_shift, size_t size, uint8_t * restrict data) {
	/**
	 * Deobfuscate data using a polyalphabetic cipher. You can also have an extra
	 * static shift character.
	 * 
	 * @param key The key that should be used to deobfuscate the data
	 * @param static_shift A static shift character (can just be zero)
	 * @param size Size of the data
	 * @param data Data buffer to decrypt
	 */
	
	size_t key_length = key ? DgStringLength(key) : 0;
	
	// If the key length is zero or NULL just use the shift character
	if (!key_length) {
		for (size_t i = 0; i < size; i++) {
			data[i] -= static_shift;
		}
		
		return;
	}
	
	// Otherwise, do the standard shift
	for (size_t i = 0; i < size; i++) {
		data[i] -= DG_POLYALPHABETIC_SHIFT();
	}
}

#undef DG_POLYALPHABETIC_SHIFT
#endif

void DgObfuscate_SEA1(const char * restrict key, size_t length, uint8_t * restrict data) {
	/**
	 * Obfuscate a message using the XorShiftShift algorithm.
	 * 
	 * @note The issues with this cipher include:
	 * 
	 * - Choosing the same key results in the same keystream expansion. There is
	 *   no protection to make sure that data encrypted with the same key can't
	 *   be analysed. (You could analyse mutlipule messages and determine the
	 *   probablity of what occurs at each point the message.)
	 * 
	 * - XORShift fails statistical tests for randomness.
	 * 
	 * - It's might be possible to brute force. There are (2^8 - 1)^L possible
	 *   keys of length L. The averege attack only needs to check about half: 
	 *   ((2^8 - 1)^L)/2. Use d for the time in seconds it takes to compute
	 *   a single encryption: ((2^8 - 1)^L)/(2 * 31536000 * d) years.
	 * 
	 * - There is a devil in these details ...
	 * 
	 * @param key Key to obfuscate with
	 * @param length Length of the data
	 * @param data Data to obfuscate
	 */
	
	size_t key_length = DgStringLength(key);
	
	// Number of random states
	uint32_t random_states[key_length];
	
	// Initialise the states...
	// This is probably the most important part of the cipher. But it might not
	// be very secure. For example there is already repetition in the key.
	for (size_t i = 0; i < key_length; i++) {
		random_states[i] =
			  (key[(i + 3) % key_length] << 24)
			| (key[(i + 0) % key_length] << 16)
			| (key[(i + 2) % key_length] << 8)
			| (key[(i + 1) % key_length]);
	}
	
	// Encrypt the data
	for (size_t i = 0; i < length; i++) {
		random_states[i % key_length] = DgPseudorandomXORShiftU32(random_states[i % key_length]);
		data[i] ^= (char)((random_states[i % key_length]) & 0xff);
	}
}

#ifdef DG_MELON_OBFUSCATE_EXTRA
void DgObfuscate_SEA2(const char * restrict key, size_t length, uint8_t * restrict data) {
	/**
	 * Obfuscate a message using the 2nd Stupid Encryption Algorithm.
	 * 
	 * @note What this does differently from SEA1:
	 * 
	 * - Adds a counter(-ish thing) based on the length of the message and bits
	 *   of the plaintext.
	 * 
	 * - I'm not sure that using the plaintext as part of the random states is
	 *   really secure but since it's only a few bits to make the keystream
	 *   depend on the plaintext I think that could be fine(?)
	 * 
	 * @param key Key to obfuscate with
	 * @param length Length of the data
	 * @param data Data to obfuscate
	 */
	
	size_t key_length = DgStringLength(key);
	
	// Number of random states
	uint32_t random_states[key_length];
	
	// Initialise the states...
	// This is probably the most important part of the cipher. But it might not
	// be very secure. For example there is already repetition in the key.
	for (size_t i = 0; i < key_length; i++) {
		random_states[i] =
			( (key[(i + 3) % key_length] << 24)
			| (key[(i + 0) % key_length] << 16)
			| (key[(i + 2) % key_length] << 8)
			| (key[(i + 1) % key_length]));
	}
	
	// Encrypt the data
	for (size_t i = 0; i < length; i++) {
		random_states[i % key_length] = DgPseudorandomXORShiftU32(random_states[i % key_length]) + (length + i);
		
		data[i] ^= (char)((random_states[i % key_length]) & 0xff);
	}
}

void DgObfuscateData(DgObfuscateAlgorithm algorithm, const char * restrict key, size_t length, uint8_t * restrict data) {
	/**
	 * Obfuscate a given block of data by using weak encryption algorithm.
	 * 
	 * @param algorithm Obfuscation algorithm to use
	 * @param key Obfuscation key to use
	 * @param length Length of the data to obfuscate
	 * @param data Data to obfuscate
	 */
	
	switch (algorithm) {
		case DG_OBFUSCATE_ROT13: {
			DgObfuscate_Rot13(length, data);
			break;
		}
		
		case DG_OBFUSCATE_CESAR: {
			DgObfuscate_Polyalphabetic(NULL, key[0], 0, data);
			break;
		}
		
		case DG_OBFUSCATE_POLYALPHABETIC: {
			DgObfuscate_Polyalphabetic(key, 0, length, data);
			break;
		}
		
		case DG_OBFUSCATE_MEDIOCRE: {
			DgObfuscate_Polyalphabetic(key, (char)(length & 0xff), length, data);
			break;
		}
		
		case DG_OBFUSCATE_SEA1: {
			DgObfuscate_SEA1(key, length, data);
			break;
		}
		
		case DG_OBFUSCATE_SEA2: {
			DgObfuscate_SEA2(key, length, data);
			break;
		}
		
		default: {
			break;
		}
	}
}

void DgDeobfuscateData(DgObfuscateAlgorithm algorithm, const char * restrict key, size_t length, uint8_t * restrict data) {
	/**
	 * Obfuscate a given block of data by using weak encryption algorithm.
	 * 
	 * @param algorithm Obfuscation algorithm to use
	 * @param key Obfuscation key to use
	 * @param length Length of the data to obfuscate
	 * @param data Data to obfuscate
	 */
	
	switch (algorithm) {
		case DG_OBFUSCATE_ROT13: {
			DgObfuscate_Rot13(length, data);
			break;
		}
		
		case DG_OBFUSCATE_CESAR: {
			DgDeobfuscate_Polyalphabetic(NULL, key[0], 0, data);
			break;
		}
		
		case DG_OBFUSCATE_POLYALPHABETIC: {
			DgDeobfuscate_Polyalphabetic(key, 0, length, data);
			break;
		}
		
		case DG_OBFUSCATE_MEDIOCRE: {
			DgDeobfuscate_Polyalphabetic(key, (char)(length & 0xff), length, data);
			break;
		}
		
		case DG_OBFUSCATE_SEA1: {
			DgObfuscate_SEA1(key, length, data);
			break;
		}
		
		case DG_OBFUSCATE_SEA2: {
			DgObfuscate_SEA2(key, length, data);
			break;
		}
		
		default: {
			break;
		}
	}
}
#else
void DgObfuscateData(const char * restrict key, size_t length, uint8_t * restrict data) {
	DgObfuscate_SEA1(key, length, data);
}

void DgDeobfuscateData(const char * restrict key, size_t length, uint8_t * restrict data) {
	DgObfuscate_SEA1(key, length, data);
}
#endif
