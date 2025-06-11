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

typedef struct { uint8_t r, g, b; } DgRGBPixel;
typedef struct { uint8_t r, g, b, a; } DgRGBAPixel;

typedef enum : uint16_t {
	DG_TEXTURE_L = 1,
	DG_TEXTURE_LA = 2,
	DG_TEXTURE_RGB = 3,
	DG_TEXTURE_RGBA = 4,
} DgTextureFormat;

typedef struct DgTexture {
	union {
		uint8_t *pixels;
		DgRGBPixel *rgb_pixels;
		DgRGBAPixel *rgba_pixels;
	};
	uint32_t width;
	uint32_t height;
	DgTextureFormat format;
} DgTexture;

bool DgTextureLoadQOIFromStream(DgTexture *this, DgStream *stream);
bool DgTextureLoadQOI(DgTexture *this, const char *path);
bool DgTextureWriteQOGToStream(DgTexture *this, DgStream *stream);
bool DgTextureLoadTGAFromStream(DgTexture *this, DgStream *stream);
bool DgTextureGenerateTiles(DgTexture *this);
bool DgTextureGreyscale(DgTexture *this);
void DgTextureFree(DgTexture *this);
