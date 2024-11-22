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

#include "error.h"

typedef char **DgCStringArray;

enum : uint32_t {
	DG_UTF8_DECODE_ERROR = 0xffffffff,
};

size_t DgUTF8UnicodeCharLen(const uint8_t *source, size_t remain);
uint32_t DgUTF8UnicodeDecodeCharEx(const uint8_t *source, size_t remain, DgErrorCode *error, size_t *size_out);
uint32_t DgUTF8UnicodeDecodeChar(const char *source, size_t remain, size_t *size_out);

char *DgStringConcatinate(const char * const string1, const char * const string2);
char *DgStringConcatinateL(const char * const string1, const char * const string2);
char *DgStringConcatinateR(const char * const string1, const char * const string2);
char *DgStringConcatinateLR(const char * const string1, const char * const string2);
char *DgStringDuplicate(const char * const string);
char *DgStringDuplicateUntil(const char * const string, size_t length);
bool DgStringEqual(const char * const string1, const char * const string2);
bool DgStringStartsWith(const char * restrict base, const char * restrict what);
bool DgStringEndsWith(const char * restrict base, const char * restrict what);
void DgStringUppercaseInplace(char *string);
char *DgStringUppercase(const char *string);
void DgStringLowercaseInplace(char *string);
char *DgStringLowercase(const char *string);
size_t DgStringLength(const char * const string);
size_t DgStringCharacterCount(const char * const string);
char *DgStringSlice(const char *base, size_t start, size_t end);
int64_t DgStringFind(const char * const string, const char * const what, const size_t which);
int64_t DgStringFindFirst(const char * const string, const char * const what);
size_t DgStringCountConsecutiveWithMax(const uint8_t * const data, size_t length, size_t max);
char *DgIntegerToString(uint8_t base, int64_t data);
char *DgStringEncodeBase64(size_t length, const void *input_);
char *DgStringEncodeBase16(size_t length, const void *input_);
