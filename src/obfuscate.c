/**
 * Copyright (C) 2021 - 2023 Knot126 and Descentix Software
 * ========================================================
 * 
 * Data Obfuscation
 * 
 * @note These are really just weak encryption ciphers; however, we don't really
 * want people using these for encryption and they are more useful for e.g. hiding
 * your yiff from your father's grand mother than actually keeping people from
 * looking at your data.
 */

#include "string.h"

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
