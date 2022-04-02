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

enum {
	DG_BITMAP_DRAWING_ALPHA = (1 << 0),
	DG_BITMAP_DRAWING_DEPTH = (1 << 1),
	DG_BITMAP_DRAWING_PERSPECTIVE = (1 << 2),
};

typedef uint16_t DgBitmapFlags;

/**
 * Vertex
 * ======
 * 
 * A vertex used for specifying coloured and textured triangles and points.
 */
typedef struct DgBitmapVertex {
	DgVec3 position;
	DgVec2 texture;
	DgColour colour;
} DgBitmapVertex;

/**
 * Triangle
 * ========
 * 
 * A single triangle.
 */
typedef struct DgBitmapTriangle {
	union {
		struct {
			DgBitmapVertex p1, p2, p3;
		};
		DgBitmapVertex data[3];
	};
} DgBitmapTriangle;

/**
 * Bitmap
 * ======
 * 
 * An 8-bit image with up to four channels: r, g, b and alpha. Also contains an
 * optional depth buffer.
 */
typedef struct DgBitmap {
	size_t alloc_sz;
	uint8_t *src;
	float *depth;
	uint16_t width;
	uint16_t height;
	uint16_t chan;
	DgBitmapFlags flags;
} DgBitmap;

bool DgBitmapInit(DgBitmap *bitmap, const uint16_t width, const uint16_t height, const uint16_t chan);
bool DgBitmapNew(DgBitmap *bitmap, const uint16_t width, const uint16_t height, const uint16_t chan);
void DgBitmapFree(DgBitmap *bitmap);
void DgBitmapSetFlags(DgBitmap *this, DgBitmapFlags flags);
DgBitmapFlags DgBitmapGetFlags(DgBitmap *this);
void DgBitmapSetDepthBuffer(DgBitmap *this, bool enable);
void DgBitmapDrawPixel(DgBitmap *this, uint16_t x, uint16_t y, DgVec4 colour);
void DgBitmapGetPixel(DgBitmap * restrict this, uint16_t x, uint16_t y, DgVec4 * restrict colour);
void DgBitmapDrawLine(DgBitmap * restrict this, DgVec2 pa, DgVec2 pb, DgColour *colour);
void DgBitmapDrawPoint(DgBitmap *this, float x, float y, float r, DgVec4 colour);
void DgBitmapDrawConvexPolygon(DgBitmap * restrict this, size_t points_count, DgVec2 * restrict points, DgVec4 * restrict colour);
void DgBitmapDrawTriangles(DgBitmap * restrict this, size_t count, DgBitmapTriangle * restrict triangles);
void DgBitmapFill(DgBitmap * restrict this, DgVec4 colour);
void DgBitmapWritePPM(DgBitmap *this, const char * const filepath);
