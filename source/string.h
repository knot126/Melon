/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * String Utilities
 */ 

#pragma once

#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

typedef enum DgBase32Type {
	DG_BASE32_TYPE_RFC = 0,
	DG_BASE32_TYPE_HEX = 1,
} DgBase32Type;

char *DgStringConcatinate(const char * const string1, const char * const string2);
char *DgStringConcatinateL(const char * const string1, const char * const string2);
char *DgStringConcatinateR(const char * const string1, const char * const string2);
char *DgStringConcatinateLR(const char * const string1, const char * const string2);
char *DgStringDuplicate(const char * const string);
char *DgStringDuplicateUntil(const char * const string, size_t length);
bool DgStringEqual(const char * const string1, const char * const string2);
bool DgStringStartsWith(const char * restrict base, const char * restrict what);
size_t DgStringLength(const char * const string);
char *DgStringSlice(const char *base, size_t start, size_t end);
int64_t DgStringFind(const char * const string, const char * const what, const size_t which);
size_t DgStringCountConsecutiveWithMax(const uint8_t * const data, size_t length, size_t max);
uint32_t DgStringSeminise(const char *string);
const char *DgIntegerToString(uint8_t base, int64_t data);
char *DgStringEncodeBase64(size_t length, const void *input_);
char *DgStringEncodeBase16(size_t length, const void *input_);

#if defined(DG_MELON_OLD_STRING_FUNCTIONS)
	char *DgStrcad(const char * const src1, const char * const src2);
	char *DgStrcadf(char *src1, const char * const src2);
	char *DgStrdup(const char * const source);
	char *DgStrdupl(const char * const source, size_t max);
	size_t DgStrspn(const char * restrict src, const char * const restrict delim);
	size_t DgStrcspn(const char * restrict src, const char * const restrict delim);
	char *DgStrtokr(char *src, const char * const restrict delim, char **saveptr);
	uint32_t *DgInt32ListFromString(char * restrict str, size_t * const restrict size);
#endif
