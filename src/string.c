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
 * String utilities
 * 
 * @todo Unicode, strings without null terminators
 */ 

#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "alloc.h"
#include "memory.h"
#include "log.h"

#include "string.h"

#if defined(DG_MELON_OLD_STRING_FUNCTIONS)
char *DgStrcad(const char * const src1, const char * const src2) {
	/**
	 * Concatinate two strings into one, the resulting memory must be freed
	 * manually.
	 */
	
	uint32_t len1 = strlen(src1);
	uint32_t len2 = strlen(src2);
	
	char *ns = DgAlloc((len1 + len2 + 1) * sizeof(char));
	
	memcpy(ns, src1, len1);
	memcpy(ns + len1, src2, len2);
	ns[len1 + len2] = '\0';
	
	return ns;
}

char *DgStrcadf(char *src1, const char * const src2) {
	/*
	 * Same as DgStrcad, but first string is freed after concat. This is useful
	 * for compacting concatenations.
	 */
	char *string = DgStrcad(src1, src2);
	
	DgFree(src1);
	
	return string;
}

char *DgStrdup(const char * const source) {
	/*
	 * Duplicate a string
	 */
	
	char *dest = DgAlloc(strlen(source) + 1);
	
	if (!dest) {
		return NULL;
	}
	
	return strcpy(dest, source);
}

char *DgStrdupl(const char * const source, size_t max) {
	/*
	 * Duplicate a string up to a certian length
	 */
	size_t len = 0;
	
	// Get size
	while (len < max && source[len] != '\0') {
		len++;
	}
	
	// Allocate memory
	char *dest = DgAlloc(len + 1);
	
	if (!dest) {
		return NULL;
	}
	
	// Copy bytes
	for (size_t i = 0; i < len; i++) {
		dest[i] = source[i];
	}
	
	// Set last to null terminator
	dest[len] = '\0';
	
	// Return new string
	return dest;
}

size_t DgStrspn(const char * restrict src, const char * const restrict delim) {
	/**
	 * Implementation of strspn()
	 * 
	 * Returns the number of chars in the string up until one is found that does
	 * not match those in delim. 
	 * 
	 * For example, if you want to find the first non-number char in a string:
	 * 
	 * char myString[] = "234.630f";
	 * myString += DgStrspn(myString, "0123456789");
	 * myString now points to the '.', so its now ".630f"
	 * 
	 * The difference between this one and the one with the 'c' is that this one
	 * has a logical not in the found test.
	 */
	
	size_t delim_count = strlen((char *) delim);
	size_t ret = 0;
	
	while (*src != '\0') {
		bool found = false;
		
		for (size_t i = 0; i < delim_count; i++) {
			if (*src == delim[i]) {
				found = true;
				break;
			}
		}
		
		if (!found) {
			break;
		}
		
		src++;
		ret++;
	}
	
	return ret;
}

size_t DgStrcspn(const char * restrict src, const char * const restrict delim) {
	/**
	 * Implementation of strcspn()
	 * 
	 * Returns the number of chars in the string up until one is found that does
	 * match those in delim. 
	 * 
	 * For example, if you want to find the number of chars before 'e':
	 * 
	 * char myString[] = "Fox of the Woods";
	 * size_t before_e = DgStrcspn(myString, "e");
	 * 
	 * The difference between this and the one without the 'c' is the match test
	 * not having a logical not before it.
	 */
	
	size_t delim_count = strlen((char *) delim);
	size_t ret = 0;
	
	while (*src != '\0') {
		bool match = false;
		
		for (size_t i = 0; i < delim_count; i++) {
			if (*src == delim[i]) {
				match = true;
				break;
			}
		}
		
		if (match) {
			break;
		}
		
		src++;
		ret++;
	}
	
	return ret;
}

char *DgStrtokr(char *src, const char * const restrict delim, char **saveptr) {
	/**
	 * Custom implementation of strtok_r() with a different name
	 * 
	 * This helps split strings into tokens.
	 */
	
	// Get state from the last run if src is NULL
	if (!src) {
		src = *saveptr;
	}
	
	// Skip to first char that is not a delimiter
	src += DgStrspn(src, delim);
	
	// Out of chars
	if (*src == '\0') {
		return NULL;
	}
	
	// We are now at the string we need to return, so set ret to that string
	char * const ret = src;
	
	// Go up to the next delimiter and set that delimiter to null
	src += DgStrcspn(src, delim);
	*src = '\0';
	
	// Prepare for next time by skipping past the null into the next char
	if (saveptr) {
		src++;
		*saveptr = src;
	}
	
	return ret;
}

uint32_t *DgInt32ListFromString(char * restrict str, size_t * const restrict size) {
	/**
	 * Converts a string to a list of 32-bit integers, writes the size of the
	 * list out to (size_t) size.
	 */
	
	if (!str) {
		return NULL;
	}
	
	*size = (size_t) 0;
	
	char *next = NULL;
	uint32_t *list = NULL;
	
	while (true) {
		uint32_t result = strtol(str, &next, 10);
		
		if (str == next) {
			break;
		}
		
		(*size)++;
		list = DgRealloc(list, size[0] * sizeof(uint32_t));
		
		if (!list) {
			return NULL;
		}
		
		list[(*size) - 1] = result;
		
		str = next;
	}
	
	return list;
}
#endif

/** ============================================================================
 * 
 * New string functions
 * 
 *  ============================================================================
 */

char *DgStringConcatinate(const char * const string1, const char * const string2) {
	/**
	 * Concatinate string1 and string2
	 * 
	 * @warning You need to free the string returned by this function.
	 * 
	 * @note The result is defined if one or both strings are NULL:
	 * 
	 *   DgStringConcatinate(string, NULL) = NULL
	 *   DgStringConcatinate(NULL, NULL) = NULL
	 * 
	 * I considered making concat(str, NULL) == dup(str), but consider the
	 * following case:
	 * 
	 *   concat(concat("a", "b"), concat("c", "d"))
	 *          ^^ works ^^^^^^^  ^^ fails ^^^^^^^
	 *   ^^ works ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	 * 
	 * That would mean you get the string "ab" when you were expecting "abcd"!
	 * That's a harder error to find, and it's not something you could
	 * reasonably expect someone to test for, so instead we always return NULL.
	 * 
	 * @param string1 First string
	 * @param string2 Second string
	 * @return Resulting string (or NULL if failed)
	 */
	
	if (!string1 || !string2) {
		return NULL;
	}
	
	size_t length1 = DgStringLength(string1);
	size_t length2 = DgStringLength(string2);
	
	char *result = DgAlloc(length1 + length2 + 1);
	
	if (!result) {
		return NULL;
	}
	
	// Copy bytes in first string
	for (size_t i = 0; i < length1; i++) {
		result[i] = string1[i];
	}
	
	// Copy bytes in second string
	for (size_t i = length1; i < (length1 + length2); i++) {
		result[i] = string2[i - length1];
	}
	
	// Set last to NUL byte
	result[length1 + length2] = '\0';
	
	return result;
}

char *DgStringConcatinateL(const char * const string1, const char * const string2) {
	/**
	 * Concatinate string1 and string2 and then free string1
	 * 
	 * @warning You need to free the string returned by this function.
	 * 
	 * @note If the concatenation fails, the left string will still be freed.
	 * 
	 * @param string1 First string
	 * @param string2 Second string
	 * @return Resulting string (or NULL if failed)
	 */
	
	char *result = DgStringConcatinate(string1, string2);
	
	DgFree((void *) string1);
	
	return result;
}

char *DgStringConcatinateR(const char * const string1, const char * const string2) {
	/**
	 * Concatinate string1 and string2 and then free string2
	 * 
	 * @warning You need to free the string returned by this function.
	 * 
	 * @note If the concatenation fails, the right string will still be freed.
	 * 
	 * @param string1 First string
	 * @param string2 Second string
	 * @return Resulting string (or NULL if failed)
	 */
	
	char *result = DgStringConcatinate(string1, string2);
	
	DgFree((void *) string2);
	
	return result;
}

char *DgStringConcatinateLR(const char * const string1, const char * const string2) {
	/**
	 * Concatinate string1 and string2 and then free both strings
	 * 
	 * @warning You need to free the string returned by this function.
	 * 
	 * @note If the concatenation fails, the strings will still be freed.
	 * 
	 * @param string1 First string
	 * @param string2 Second string
	 * @return Resulting string (or NULL if failed)
	 */
	
	char *result = DgStringConcatinate(string1, string2);
	
	DgFree((void *) string1);
	DgFree((void *) string2);
	
	return result;
}

size_t DgStringLength(const char * const string) {
	/**
	 * Return the length of the given string
	 * 
	 * @note This function currently causes undefined behaviour when passed NULL.
	 * 
	 * @param string String to check length of
	 * @return Length of string
	 */
	
	for (size_t i = 0;; i++) {
		if (string[i] == '\0') {
			return i;
		}
	}
}

char *DgStringDuplicate(const char * const string) {
	/**
	 * Make a copy of a string in a new section of memory
	 * 
	 * @note If string is NULL, this will return NULL
	 * 
	 * @param string String to duplicate
	 * @return Dupilcated string (or NULL if failure or string was NULL)
	 */
	
	// If string is NULL then just return NULL
	if (!string) {
		return NULL;
	}
	
	// Length of memory to allocate
	size_t length = DgStringLength(string) + 1;
	
	// Allocate string memory
	char *result = DgAlloc(length);
	
	if (!result) {
		return NULL;
	}
	
	// Copy string data
	for (size_t i = 0; i < length; i++) {
		result[i] = string[i];
	}
	
	return result;
}

char *DgStringDuplicateUntil(const char * const string, size_t length) {
	/**
	 * Duplicate a string up to (but not including) the `length`th character.
	 * 
	 * @warning You need to free the string returned by this function.
	 * 
	 * @note If string is NULL, then this function will simply return NULL.
	 * 
	 * @param string String to duplicate
	 * @param length Number of characters to duplicate
	 * @return Duplicated string
	 */
	
	// If the base string isn't then don't.
	if (!string) {
		return NULL;
	}
	
	size_t string_length = DgStringLength(string);
	
	// Use the length of the normal string if it's less
	length = (string_length < length) ? (string_length) : (length);
	
	// Allocate memory for the string
	char *result = DgMemoryAllocate(length + 1);
	
	if (!result) {
		return NULL;
	}
	
	// Copy the string up to the `length`th character
	for (size_t i = 0; i < length; i++) {
		result[i] = string[i];
	}
	
	// Set the last byte to the NUL char
	result[length] = '\0';
	
	return result;
}

bool DgStringEqual(const char * const string1, const char * const string2) {
	/**
	 * Check if the two strings are equal.
	 * 
	 * @note If either string is a NULL pointer, only the values of the pointers
	 * are checked for equality. This results in:
	 * 
	 *   DgStringEqual(valid_string, NULL) == false
	 *   DgStringEqual(NULL, NULL) == true
	 * 
	 * Which is probably the most logical behaviour. This could change in the
	 * future if there is a good reason to change it.
	 * 
	 * @param string1 First string
	 * @param string2 Second string
	 * @return If (string1 == string2)
	 */
	
	// If one string is NULL, then it's only true if both are NULL.
	if (!string1 || !string2) {
		return string1 == string2;
	}
	
	for (size_t i = 0;; i++) {
		// Check the current character
		if (string1[i] == string2[i]) {
			// They are the same!
			
			// If we are on the NUL byte, then we are done!
			if (string1[i] == '\0') {
				return true;
			}
			
			// Otherwise we continue
			continue;
		}
		else {
			// They are not the same. Note that this handles the case where one
			// string is shorter than another well.
			return false;
		}
	}
}

bool DgStringStartsWith(const char * restrict base, const char * restrict what) {
	/**
	 * Check if `base` starts with `what`.
	 * 
	 * @param base String to search in
	 * @param what String to search for
	 * @return true if base starts with what and false if not
	 */
	
	size_t base_length = DgStringLength(base);
	size_t what_length = DgStringLength(what);
	
	if (what_length > base_length) {
		return false;
	}
	
	for (size_t i = 0; i < base_length; i++) {
		if (base[i] != what[i] && what[i] != '\0') {
			return false;
		}
		else if (what[i] == '\0') {
			break;
		}
	}
	
	return true;
}

char *DgStringSlice(const char *base, size_t start, size_t end) {
	/**
	 * Allocate a slice of a string given a string.
	 * 
	 * @note Unusually, bounds *will* be checked on input indexes.
	 * 
	 * @warning You need to free the string returned when you are done with it.
	 * 
	 * @param base The base string
	 * @param start The starting index of the substring (inclusive)
	 * @param end The ending index of the substring (exclusive)
	 * @return Allocated substring
	 */
	
	// Check that everything is okay
	size_t length = DgStringLength(base);
	
	if (start > length || end > length) {
		return NULL;
	}
	
	// Find size of slice
	size_t size = end - start;
	
	// Small hack :3
	char *result = DgStringDuplicateUntil(base + start, size);
	
	return result;
}

int64_t DgStringFind(const char * const string, const char * const what, size_t which) {
	/**
	 * Find the `which`th substring in a given string.
	 * 
	 * @param string The string to search for the substring
	 * @param what The string to find
	 * @param which How many found substrings should be skipped
	 * @return The index of the substring, or -1 if not found
	 */
	
	size_t length = DgStringLength(string);
	
	for (size_t i = 0; i < length; i++) {
		bool result = DgStringStartsWith(&string[i], what);
		
		if (result && (which--) == 0) {
			return i;
		}
	}
	
	return -1;
}

int64_t DgStringFindFirst(const char * const string, const char * const what) {
	/**
	 * Find the first occurance of "what" in "string"
	 * 
	 * @param string String to search for the substring
	 * @param what String to find
	 * @return The index of the substring, or -1 if not found
	 */
	
	return DgStringFind(string, what, 0);
}

uint32_t DgStringSeminise(const char *string) {
	/**
	 * Take the "sem" (our word for small, non-cryptographic hash) of a string.
	 * 
	 * @note The algorithm used is DJB2 (xor version), but it can change.
	 * 
	 * @note DJB2 (Xor version) algorithm: hash[i] = (33 * hash[i - 1]) ^ string[i]
	 * 
	 * @see http://www.cse.yorku.ca/~oz/hash.html
	 * 
	 * @param string The string to seminise
	 * @return Sem of the string
	 */
	
	uint32_t hash = 5381;
	size_t i = 0;
	
	while (string[i] != '\0') {
		//     33 * hash            ^ string[i];
		hash = ((hash << 5) + hash) ^ string[i];
		i++;
	}
	
	return hash;
}

const char gBase64EncodeTable[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
	'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
	'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
	'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '_',
};

char *DgStringEncodeBase64(size_t length, const void *input_) {
	/**
	 * Encode a piece of data to base64.
	 * 
	 * @note The output will be dynamically allocated and must be freed.
	 * 
	 * @param length The length of the data to encode
	 * @param input_ The input data
	 * @return Output string
	 * 
	 * @see https://datatracker.ietf.org/doc/html/rfc4648.html
	 */
	
	const uint8_t *input = (const uint8_t *) input_;
	
	size_t leftover = length % 3;
	
	// Per the RFC, we need to account for the following:
	// leftover == 0  ->  +0 (no ='s)
	// leftover == 1  ->  +2 + leftover (for ='s, leftover = 1)
	// leftover == 2  ->  +3 + leftover (for ='s, leftover = 2)
	size_t output_length = ((length / 3) * 4) + ((leftover == 1) ? 3 : 0) + ((leftover == 2) ? 5 : 0) + 1;
	
	char *output = DgMemoryAllocate(output_length);
	
	if (!output) {
		return NULL;
	}
	
	// The main loop, as suggested by RFC 4648
	size_t i = 0, j = 0;
	
	for (; i < length; i += 3, j += 4) {
		// Convert to a 24-bit integer, concat the bytes in big endian
		uint32_t inter = (input[i] << 16) | (input[i + 1] << 8) | (input[i + 2]);
		
		// Split them into 6-bit integers
		uint8_t a = (inter >> 18) & 0b111111;
		uint8_t b = (inter >> 12) & 0b111111;
		uint8_t c = (inter >> 6 ) & 0b111111;
		uint8_t d = (inter >> 0 ) & 0b111111;
		
		// Index the base64 alphabet
		output[j + 0] = gBase64EncodeTable[a];
		output[j + 1] = gBase64EncodeTable[b];
		output[j + 2] = gBase64EncodeTable[c];
		output[j + 3] = gBase64EncodeTable[d];
	}
	
	// If we have leftover items, we need to take care of them...
	if (leftover) {
		// There will ALWAYS be a first item if we are here, and maybe a second
		// but this only handles two special cases.
		uint32_t inter = (input[i] << 16) | (((leftover == 2) ? input[i + 1] : 0) << 8);
		
		// This part is normal
		uint8_t a = (inter >> 18) & 0b111111;
		uint8_t b = (inter >> 12) & 0b111111;
		uint8_t c = (inter >> 6 ) & 0b111111;
		uint8_t d = (inter >> 0 ) & 0b111111;
		
		output[j + 0] = gBase64EncodeTable[a];
		output[j + 1] = gBase64EncodeTable[b];
		
		// Per the RFC, we don't include any chars we don't need
		if (leftover >= 1) {
			output[j + 2] = gBase64EncodeTable[c];
			
			if (leftover >= 2) {
				output[j + 3] = gBase64EncodeTable[d];
			}
		}
		
		// We will always have to write this equal
		output[output_length - 2] = '=';
		
		// If we have two leftover bytes, then we write a second equals
		if (leftover == 2) {
			output[output_length - 3] = '=';
		}
	}
	
	output[output_length - 1] = '\0';
	
	return output;
}

const char gStringEncodeBase32TableRfc[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
	'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '2', '3', '4', '5', '6', '7',
};

const char gStringEncodeBase32TableHex[] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
	'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
};

char *DgStringEncodeBase32(DgBase32Type type, size_t length, const void *input_) {
	/**
	 * Encode bytes to base32 or base32hex
	 * 
	 * @note You should really use either Base64 for compactness or Base16 for
	 * readability. This functions are only implemented for fun. :)
	 * 
	 * @param type The type of Base32 encoding (RFC 4648 or Hex)
	 * @param length The length of the data to encode
	 * @param input The data to encode
	 * @return Base32 encoded string
	 */
	
#if 0
	const uint8_t *input = (const uint8_t *) input_;
	
	const char *alphabet = (type == DG_BASE32_TYPE_RFC) ? gStringEncodeBase32TableRfc : gStringEncodeBase32TableHex;
	
	size_t leftover = length % 5;
	
	for (size_t i = 0; i < length; i++) {
		// unfinsihed
	}
#endif
	
	return NULL;
}

const char gStringEncodeBase16Table[] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
};

char *DgStringEncodeBase16(size_t length, const void *input_) {
	/**
	 * Encode the given data to a hex string.
	 * 
	 * @note Base64 is more efficent, so unless you need some human readability,
	 * please consider using that instead.
	 * 
	 * @param length Length of the data to convert to hex
	 * @param input_ Input data pointer
	 * @return Base16 string
	 */
	
	const uint8_t *input = (const uint8_t *) input_;
	
	// Thankfully this is a lot simpler than base64, becuase 4-bits fit evenly
	// into 8-bits.
	size_t output_length = 2 * length + 1;
	
	// Try to allocate the memory
	char *output = DgMemoryAllocate(output_length);
	
	if (!output) {
		return NULL;
	}
	
	// Encode it!
	for (size_t i = 0; i < length; i++) {
		// Higher four bits
		output[(2 * i) + 0] = gStringEncodeBase16Table[input[i] >> 4];
		
		// Lower four bits
		output[(2 * i) + 1] = gStringEncodeBase16Table[input[i] & 0b1111];
	}
	
	// Place the NUL byte
	output[output_length - 1] = '\0';
	
	return output;
}
