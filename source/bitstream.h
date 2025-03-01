/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Bit oriented streams
 */

#pragma once

#include "stream.h"

typedef struct DgBitStream {
	DgStream *base; // The byte oriented stream to write out to
	uint8_t queued; // The pending byte to write out, where the most significant bits are valid
	uint8_t amount_queued; // The number of valid bits in the pending byte
	uint8_t queued_in; // The pending output byte (for reading)
	uint8_t amount_queued_in; // The available input bytes
} DgBitStream;
