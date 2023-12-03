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
 * Implementation of DJB's CubeHash
 */

#pragma once

typedef struct DgCryptoCubeHasher {
	uint32_t state[32];
	uint32_t initial, roundsperblock, bytesperblock, finishing, outputlen;
} DgCryptoCubeHasher;

// Streaming API with customisable parameters
DgError DgCryptoCubeHasherInit(DgCryptoCubeHasher *this, uint32_t i, uint32_t r, uint32_t b, uint32_t f, uint32_t h);
DgError DgCryptoCubeHasherNextBlock(DgCryptoCubeHasher *this, size_t length, const uint8_t *block);
DgError DgCryptoCubeHasherFinalise(DgCryptoCubeHasher *this, size_t * const length, uint8_t ** const hash);

// Direct API with customisable parameters
uint8_t *DgCryptoCubeHashBytes(const size_t length, const uint8_t *block, uint32_t i, uint32_t r, uint32_t b, uint32_t f, uint32_t h);

// Simple API
uint8_t *DgCryptoCubeHash512(const size_t length, const uint8_t *input);
uint8_t *DgCryptoCubeHash384(const size_t length, const uint8_t *input);
uint8_t *DgCryptoCubeHash256(const size_t length, const uint8_t *input);
