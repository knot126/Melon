/*
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * Bitmaps and Other Images
 */  

#pragma once

#include <stdbool.h>
#include <inttypes.h>

#include "maths.h"

typedef struct DgBitmap {
	size_t alloc_sz;
	uint8_t *src;
	uint16_t width;
	uint16_t height;
	uint16_t chan;
	uint16_t flags;
} DgBitmap;

bool DgBitmapInit(DgBitmap *bitmap, const uint16_t width, const uint16_t height, const uint16_t chan);
bool DgBitmapNew(DgBitmap *bitmap, const uint16_t width, const uint16_t height, const uint16_t chan);
void DgBitmapFree(DgBitmap *bitmap);
void DgBitmapDrawPixel(DgBitmap *this, uint16_t x, uint16_t y, DgVec4 colour);
void DgBitmapGetPixel(DgBitmap * restrict this, uint16_t x, uint16_t y, DgVec4 * restrict colour);
void DgBitmapDrawPoint(DgBitmap *this, float x, float y, float r, DgVec4 colour);
void DgBitmapDrawConvexPolygon(DgBitmap * restrict this, size_t points_count, DgVec2 * restrict points, DgVec4 * restrict colour);
void DgBitmapFill(DgBitmap * restrict this, DgVec4 colour);
void DgBitmapAntiAliasX(DgBitmap *this);
void DgBitmapWritePPM(DgBitmap *this, const char * const filepath);

DgBitmap *DgBitmapGenTiles(const uint16_t width, const uint16_t height, const uint16_t size);
DgBitmap *DgBitmapRandom(const uint16_t width, const uint16_t height);
