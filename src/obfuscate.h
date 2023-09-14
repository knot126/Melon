/**
 * Copyright (C) 2021 - 2023 Knot126 and Descentix Software
 * ========================================================
 * 
 * Data Obfuscation (aka weak encryption)
 */

#pragma once

#include <inttypes.h>

#ifdef DG_MELON_OBFUSCATE_EXTRA
enum {
	DG_OBFUSCATE_NONE = 0,
	DG_OBFUSCATE_ROT13,
	DG_OBFUSCATE_CESAR,
	DG_OBFUSCATE_POLYALPHABETIC,
	DG_OBFUSCATE_MEDIOCRE,
	DG_OBFUSCATE_SEA1,
	DG_OBFUSCATE_SEA2,
};

typedef int32_t DgObfuscateAlgorithm;

typedef struct DgObfuscateSEA3Key {
	char bytes[10];
} DgObfuscateSEA3Key;

void DgObfuscate_Rot13(size_t length, uint8_t * restrict data);
void DgObfuscate_Substitution(const uint8_t * const restrict table, size_t length, uint8_t * restrict data);
void DgObfuscate_Polyalphabetic(const char * restrict key, char static_shift, size_t size, uint8_t * restrict data);
void DgDeobfuscate_Polyalphabetic(const char * restrict key, char static_shift, size_t size, uint8_t * restrict data);
void DgObfuscate_SEA1(const char * restrict key, size_t length, uint8_t * restrict data);
void DgObfuscate_SEA2(const char * restrict key, size_t length, uint8_t * restrict data);

void DgObfuscateData(DgObfuscateAlgorithm algorithm, const char * restrict key, size_t length, uint8_t * restrict data);
void DgDeobfuscateData(DgObfuscateAlgorithm algorithm, const char * restrict key, size_t length, uint8_t * restrict data);
#else
void DgObfuscateData(const char * restrict key, size_t length, uint8_t * restrict data);
void DgDeobfuscateData(const char * restrict key, size_t length, uint8_t * restrict data);
#endif
