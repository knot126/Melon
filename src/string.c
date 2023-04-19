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
 * String Utilities
 */ 

#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "alloc.h"
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
	 * @param string1 First string
	 * @param string2 Second string
	 * @return Resulting string (or NULL if failed)
	 */
	
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

size_t DgStringLength(const char * const string) {
	/**
	 * Return the length of the given string
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
	/*
	 * Duplicate a string
	 */
	
	size_t length = DgStringLength(string) + 1;
	
	char *result = DgAlloc(length);
	
	if (!result) {
		return NULL;
	}
	
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
	 * @param string String to duplicate
	 * @param length Number of characters to duplicate
	 * @return Duplicated string
	 */
	
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
	 * Check if the two strings are equal
	 * 
	 * @param string1 First string
	 * @param string2 Second string
	 * @return If (string1 == string2)
	 */
	
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
	 * @warning You need to free the string returned when you are done with it.
	 * 
	 * @param base The base string
	 * @param start The starting index of the substring (inclusive)
	 * @param end The ending index of the substring (exclusive)
	 * @return Allocated substring
	 */
	
	return NULL;
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

const char gBase64EncodeTable[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
	'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
	'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
	'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '_',
};

char *DgStringEncodeBase64(size_t length, uint8_t *input) {
	/**
	 * Encode a piece of data to base64.
	 * 
	 * @note The output will be dynamically allocated and must be freed.
	 * 
	 * @param length The length of the data to encode
	 * @param input The input data
	 * @return Output string
	 */
	
	size_t leftover = length % 3;
	size_t output_length = ((length / 3) * 4) + 1;
	
	char *output = DgMemoryAllocate(output_length);
	
	for (size_t i = 0, j = 0; i < length; i += 3, j += 4) {
		uint32_t inter = (input[i] << 16) | (input[i + 1] << 8) | (input[i + 2]);
		
		uint8_t a = (inter >> 18) & 0b111111;
		uint8_t b = (inter >> 12) & 0b111111;
		uint8_t c = (inter >> 6 ) & 0b111111;
		uint8_t d = (inter >> 0 ) & 0b111111;
		
		output[j + 0] = gBase64EncodeTable[a];
		output[j + 1] = gBase64EncodeTable[b];
		output[j + 2] = gBase64EncodeTable[c];
		output[j + 3] = gBase64EncodeTable[d];
	}
	
	output[output_length - 1] = '\0';
	
	return output;
}
