/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Images and textures
 */

#pragma once

#include "common.h"
#include "stream.h"

typedef enum : uint16_t {
	DG_TEXTURE_L = 1,
	DG_TEXTURE_LA = 2,
	DG_TEXTURE_RGB = 3,
	DG_TEXTURE_RGBA = 4,
} DgTextureFormat;

typedef struct DgTexture {
	union {
		uint8_t *pixels;
		uint16_t *pixels16;
		uint32_t *pixels32;
		float *pixelsf;
	};
	uint32_t width;
	uint32_t height;
	DgTextureFormat format;
} DgTexture;
