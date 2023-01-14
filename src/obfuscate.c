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
 * @note These are really just weak encryption ciphers; however, we don't really
 * want people using these for encryption and they are more useful for e.g. hiding
 * your yiff from your father's grand mother than actually keeping people from
 * looking at your data.
 */

#include "string.h"
#include "rand.h"

#include "obfuscate.h"

#define DG_POLYALPHABETIC_SHIFT() (key[i % key_length] + static_shift)

void DgPolyalphabeticObfuscate(const char * restrict key, char static_shift, size_t size, uint8_t * restrict data) {
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

void DgPolyalphabeticDeobfuscate(const char * restrict key, char static_shift, size_t size, uint8_t * restrict data) {
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

void DgObfuscateXorShiftShift(char *key, size_t length, uint8_t *data) {
	/**
	 * Obfuscate a message using the XorShiftShift algorithm.
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
		random_states[i] = (key[(i + 3) % key_length] << 24) | (key[(i + 0) % key_length] << 16) | (key[(i + 2) % key_length] << 8) | (key[(i + 1) % key_length]);
	}
	
	// Encrypt the data
	for (size_t i = 0; i < length; i++) {
		random_states[i % key_length] = DgRandXORShiftU32(random_states[i % key_length]);
		data[i] ^= (char)((random_states[i % key_length]) & 0xff);
	}
}
