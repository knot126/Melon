/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
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

#include "memory.h"
#include "log.h"
#include "error.h"

#include "string.h"

size_t DgUTF8UnicodeCharLen(const uint8_t *source, size_t remain) {
	/**
	 * Return the length of the UTF-8 encoded Unicode character starting at
	 * `source` based on its first byte. Returns zero on invalid Unicode
	 * seqences.
	 * 
	 * @param source Start of character to decode
	 * @param remain Maximum number of characters that may be read from the
	 * source.
	 * @return Number of bytes that SHOULD be in the unicode character, or 0 if
	 * the byte isn't a valid UTF-8 encoded Unicode start byte.
	 */
	
	if (remain == 0) { return 0; }
	
	uint8_t c = source[0];
	
	if (!(c & 0b10000000)) {
		return 1;
	}
	else if ((c >> 5) == 0b110) {
		if (c < 0xC2) {
			return 0;
		}
		
		return 2;
	}
	else if ((c >> 4) == 0b1110) {
		return 3;
	}
	else if ((c >> 3) == 0b11110) {
		if (c > 0xF4) {
			return 0;
		}
		
		return 4;
	}
	
	return 0;
}

uint32_t DgUTF8UnicodeDecodeCharEx(const uint8_t *source, size_t remain, DgErrorCode *error, size_t *size_out) {
	/**
	 * Decode the first UTF-8 encoded Unicode character pointed to by source.
	 * 
	 * @note Always returns 0xFFFD (replacement character) on error.
	 * 
	 * @note This decoder handles errors in the following ways:
	 *   - Continution bytes at the start of a character are 1 byte errors, so
	 *     consecutive continutation bytes are different errors.
	 *   - Bytes that never appear in UTF-8 are 1 byte errors.
	 *   - A truncated character is a 1 byte error.
	 *   - Any valid four byte character over code point 0x10FFFF is a four
	 *     byte error.
	 *   - Any valid but overly long encoding is the number of bytes of the
	 *     overlong encoding.
	 * 
	 * @param source Pointer to the start of the character to decode
	 * @param remain Length of the source that remains
	 * @param error Pointer to an error code where any specific error will be
	 * written, or DG_SUCCESS on success. If NULL, no error code is written.
	 * @param size_out Pointer to where the size will be written, if not NULL
	 * @return Valid Unicode character code
	 */
	
	if (remain == 0) {
		if (size_out) { size_out[0] = 0; }
		if (error) { error[0] = DG_UTF8_DECODE_ERROR; }
		return 0xfffd;
	}
	
	size_t utf8len = DgUTF8UnicodeCharLen(source, remain);
	
	// If size is zero or above remaining amount then error
	if (!utf8len || remain < utf8len) {
		if (size_out) { size_out[0] = 1; }
		if (error) { error[0] = DG_UTF8_DECODE_ERROR; }
		return 0xfffd;
	}
	
	uint32_t code;
	
	// Length one chars just return the first thing
	if (utf8len == 1) {
		code = source[0];
		goto finish;
	}
	
	// Some magic to get the first few bytes from the starting char.
	code = (source[0] & ((1 << (7 - utf8len)) - 1));
	
	for (size_t i = 1; i < utf8len; i++) {
		// Check for correct continutation bits
		if ((source[i] >> 6) != 0b10) {
			// I think for consisentcy with truncation at the end of a stream
			// its better for the size of the error char to always be one.
			// if (size_out) { size_out[0] = i; }
			if (size_out) { size_out[0] = 1; }
			if (error) { error[0] = DG_UTF8_DECODE_ERROR; }
			return 0xfffd;
		}
		
		// Shift over by six, insert next bytes
		code <<= 6;
		code |= source[i] & 0b111111;
	}
	
	// Enforce minimal length encoding
	if (   ((utf8len == 2) && (code < 0x80 || code > 0x7FF))
		|| ((utf8len == 3) && (code < 0x800 || code > 0xFFFF))
		|| ((utf8len == 4) && (code < 0x10000 || code > 0x10FFFF))) {
		if (size_out) { size_out[0] = utf8len; }
		if (error) { error[0] = DG_UTF8_DECODE_ERROR; }
		return 0xfffd;
	}
	
	// Success!
finish:
	if (size_out) { size_out[0] = utf8len; }
	if (error) { error[0] = DG_SUCCESS; }
	return code;
}

uint32_t DgUTF8UnicodeDecodeChar(const char *source, size_t remain, size_t *size_out) {
	return DgUTF8UnicodeDecodeCharEx((const uint8_t *) source, remain, NULL, size_out);
}

void DgUTF8UnicodeDecodeChar_Test(void) {
	const char *myString = u8"åäö 〈 test 〉 🦊 \xc0\x80ꃕ";
	size_t len = DgStringLength(myString);
	
	for (size_t i = 0; i < len;) {
		size_t charlen = 0;
		uint32_t ch = DgUTF8UnicodeDecodeChar(&myString[i], len - i, &charlen);
		DgLog(DG_LOG_INFO, "Character U+%lX (%lu) as UTF-8 is %zu bytes", ch, ch, charlen);
		i += charlen;
	}
}

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
	
	char *result = DgMemoryAllocate(length1 + length2 + 1);
	
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

void DgStringConcatinate_Test(void) {
	const char *tests[] = {
		"first", "second",
		"another ... ", "test!",
		NULL
	};
	
	for (size_t i = 0; tests[i * 2] != NULL; i++) {
		char *result = DgStringConcatinate(tests[i * 2], tests[i * 2 + 1]);
		DgLog(DG_LOG_VERBOSE, "'%s' + '%s' = '%s'", tests[i * 2], tests[i * 2 + 1], result);
		DgMemoryFree(result);
	}
	
	DgLog(DG_LOG_VERBOSE, "%s", DgStringConcatinate("test", NULL));
	DgLog(DG_LOG_VERBOSE, "%s", DgStringConcatinate(NULL, "test"));
	DgLog(DG_LOG_VERBOSE, "%s", DgStringConcatinate(NULL, NULL));
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
	
	DgMemoryFree((void *) string1);
	
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
	
	DgMemoryFree((void *) string2);
	
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
	
	DgMemoryFree((void *) string1);
	DgMemoryFree((void *) string2);
	
	return result;
}

size_t DgStringLength(const char * const string) {
	/**
	 * Return the size in bytes of the given string, not including the NUL
	 * terminator.
	 * 
	 * @note This function returns 0 when string is NULL.
	 * 
	 * @param string String to check length of
	 * @return Length of string
	 */
	
	if (!string) {
		return 0;
	}
	
	for (size_t i = 0;; i++) {
		if (string[i] == '\0') {
			return i;
		}
	}
}

size_t DgStringCharacterCount(const char * const string) {
	/**
	 * Return the number of characters in a UTF-8 encoded string.
	 * 
	 * @param string String to count characters in
	 * @return Number of characters in the string
	 */
	
	if (!string) {
		return 0;
	}
	
	const size_t string_length = DgStringLength(string);
	size_t j = 0;
	
	for (size_t i = 0; string[i] != '\0'; j++) {
		size_t char_size = 1;
		DgUTF8UnicodeDecodeChar(&string[i], string_length - i, &char_size);
		i += char_size;
	}
	
	return j;
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
	char *result = DgMemoryAllocate(length);
	
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
	 * @note If either string is NULL, then this function returns if the
	 * pointers are equal.
	 * 
	 * @param string1 First string
	 * @param string2 Second string
	 * @return If (string1 == string2)
	 */
	
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

bool DgStringEndsWith(const char * restrict base, const char * restrict what) {
	/**
	 * Check if `what` is a suffix of `base`.
	 * 
	 * @param base String to check the suffix of
	 * @param what Suffix to match
	 * @return true if base ends with what and false if not
	 */
	
	size_t base_length = DgStringLength(base);
	size_t what_length = DgStringLength(what);
	
	if (base_length < what_length) {
		return false;
	}
	
	return DgStringEqual(base + (base_length - what_length), what);
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

size_t DgStringCountConsecutiveWithMax(const uint8_t * const data, size_t length, size_t max) {
	/**
	 * Count the number of bytes that equal the first byte are at the start of
	 * the given data.
	 * 
	 * Ex: AAAAABBBAA returns 5 since there are five A's
	 * 
	 * @param data Data to use
	 * @param length Length of data
	 * @param max Max number of bytes to count
	 * @return Bytes equal to the first byte at the start of the data
	 */
	
	if (!length) {
		return 0;
	}
	
	size_t count = 0;
	
	for (size_t i = 0; i < length && i <= max; i++) {
		if (data[0] != data[i]) {
			break;
		}
		
		count++;
	}
	
	return count;
}

static bool DgCharInCharset(const char *charset, char chr) {
	/**
	 * Check if chr is in charset.
	 * 
	 * @param charset The set of characters that cause a match
	 * @param chr The character to check
	 * @return true if chr in charset, false otherwise
	 */
	
	size_t charset_length = DgStringLength(charset);
	
	for (size_t i = 0; i < charset_length; i++) {
		if (charset[i] == chr) {
			return true;
		}
	}
	
	return false;
}

DgCStringArray DgCStringSplitByWhitespace(const char * restrict string) {
	/**
	 * Split C-style string into words using whitespace as delimiters.
	 * 
	 * @param string String to split
	 * @return Strings that `string` was split into
	 */
	
	const char *whitespace = " \r\n\t\f\v";
	
	size_t result_count = 0;
	size_t i = 0;
	
	while (true) {
		// Skip whitespace
		while (DgCharInCharset(whitespace, string[i])) {
			i++;
			
			if (!string[i]) {
				break;
			}
		}
		
		if (!string[i]) {
			break;
		}
		
		// Valid chars (maybe)!
		while (!DgCharInCharset(whitespace, string[i])) {
			i++;
			
			if (!string[i]) {
				break;
			}
		}
		
		result_count++;
		
		if (!string[i]) {
			break;
		}
	}
	
	DgCStringArray result = DgMemoryAllocate(sizeof *result * (result_count + 1));
	
	if (!result) {
		return NULL;
	}
	
	i = 0;
	size_t j = 0; // Current number of results
	
	while (true) {
		// Ignore whitespace
		while (DgCharInCharset(whitespace, string[i])) {
			i++;
			
			if (!string[i]) {
				break;
			}
		}
		
		if (!string[i]) {
			break;
		}
		
		// Recognise a non-WS string and split it into another string
		size_t start = i;
		
		while (!DgCharInCharset(whitespace, string[i])) {
			i++;
			
			if (!string[i]) {
				break;
			}
		}
		
		// (end of string) - (start of string) = length of string
		size_t len = i - start;
		
		// HACK: Yes this might fail, no i don't really care atm.
		result[j] = DgStringDuplicateUntil(&string[start], len);
		
		j++;
		
		if (!string[i]) {
			break;
		}
	}
	
	// NULL to signal end of array
	result[result_count] = NULL;
	
	return result;
}

void DgCStringSplitByWhitespace_Test(void) {
	DgCStringArray result = DgCStringSplitByWhitespace("This is  some test\n\t... of my  cool thing!\n");
	
	for (size_t i = 0; result[i]; i++) {
		DgLog(DG_LOG_INFO, "result[%d] = '%s'", i, result[i]);
	}
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

const char gIntegerToStringTable[] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e',
	'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
	'u', 'v', 'w', 'x', 'y', 'z',
};

char *DgIntegerToString(uint8_t base, int64_t data) {
	/**
	 * Convert an integer to a string
	 * 
	 * @todo proper full tests, seems to work for data > 0
	 * 
	 * @param base output base of the number, between 2 and 36
	 * @param data Integer to convert
	 * @return Memory of the stringified integer
	 */
	
	if (base < 2 || base > 36) {
		return NULL;
	}
	
	// Find if it's negative
	bool negative = data < 0;
	
	// Find the amount of memory needed for our string.
	size_t string_length = 0;
	
	if (negative) { string_length++; data = -data; }
	
	int64_t temp = data;
	
	do {
		string_length++;
	} while (temp /= base);
	
	// Allocate memory
	char *out = DgMemoryAllocate(string_length + 1);
	
	if (!out) {
		return NULL;
	}
	
	// Actually do the conversion
	if (negative) {
		out[0] = '-';
	}
	
	size_t i = string_length - 1;
	
	do {
		out[i] = gIntegerToStringTable[data % base];
		i--;
	} while (data /= base);
	
	out[string_length] = '\0';
	
	return out;
}

void DgIntegerToString_Test(void) {
	int64_t tests[] = {
		10, 100,
		12, 100,
		16, 255,
		36, 23456789,
		22, 1049,
		10, 10,
		6, 10,
		3, 156,
		7, 100,
		8, 100,
		9, 100,
		11, 100,
		10, 0,
		12, 0,
		8, 0,
		10, -100,
		10, -123456,
		16, -130,
		24, -11111,
		0
	};
	
	for (size_t i = 0; tests[2 * i]; i++) {
		char *output = DgIntegerToString(tests[2 * i], tests[2 * i + 1]);
		DgLog(DG_LOG_INFO, "%lld as a string in base %d is '%s'", tests[2 * i + 1], tests[2 * i], output);
		DgMemoryFree(output);
	}
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

