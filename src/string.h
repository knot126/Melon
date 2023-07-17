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
uint32_t DgStringSeminise(const char *string);
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
