/*
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * String Utilities
 */ 

#pragma once

#include <stdlib.h>
#include <inttypes.h>

char *DgStrcad(const char * const src1, const char * const src2);
char *DgStrcadf(char *src1, const char * const src2);
char *DgStrdup(const char * const source);
char *DgStrdupl(const char * const source, size_t max);
size_t DgStrspn(const char * restrict src, const char * const restrict delim);
size_t DgStrcspn(const char * restrict src, const char * const restrict delim);
char *DgStrtokr(char *src, const char * const restrict delim, char **saveptr);
uint32_t *DgInt32ListFromString(char * restrict str, size_t * const restrict size);
