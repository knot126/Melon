/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Symmetric cryptography
 */

#pragma once

typedef struct DgCryptoChaChaState {
	size_t counter;
	size_t remaining_keystream;
	uint32_t key[8];
	uint32_t nonce[3];
	union {
		uint32_t block[16];
		uint8_t keystream[64];
	};
	uint32_t rounds;
	bool ietf;
} DgCryptoChaChaState;
