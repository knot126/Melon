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
 * Bitmaps and Other Images
 */

#pragma once

#include <stdbool.h>
#include <inttypes.h>

#include "maths.h"

enum {
	DG_BITMAP_DRAWING_ALPHA = (1 << 0), /// @deprecated
	DG_BITMAP_DRAWING_ANTIALIAS = (1 << 0),
	DG_BITMAP_DRAWING_DEPTH = (1 << 1),
	DG_BITMAP_DRAWING_PERSPECTIVE = (1 << 2),
	DG_BITMAP_DRAWING_NEGATE_Z = (1 << 3),
	DG_BITMAP_NO_CORRECT_COORDINATES = (1 << 4),
	DG_BITMAP_UNFUCK_RGB = (1 << 5), /// @fucking-stupid
	DG_BITMAP_EXTERNAL_SOURCE = (1 << 15),
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
 * Index
 * =====
 * 
 * An index used for pointing to offsets of verticies so that geometry can be
 * stored using less space.
 */
typedef struct DgBitmapIndex {
	union {
		struct {
			uint32_t a, b, c;
		};
		uint32_t data[3];
	};
} DgBitmapIndex;

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
	uint8_t *src;
	float *depth;
	uint16_t width;
	uint16_t height;
	uint16_t chan;
	DgBitmapFlags flags;
} DgBitmap;

bool DgBitmapInit(DgBitmap *bitmap, const uint16_t width, const uint16_t height, const uint16_t chan);
void DgBitmapFree(DgBitmap *bitmap);
void DgBitmapSetSource(DgBitmap * restrict this, uint8_t * restrict source, DgVec2I size, uint16_t channels);
void DgBitmapSetFlags(DgBitmap *this, DgBitmapFlags flags);
DgBitmapFlags DgBitmapGetFlags(DgBitmap *this);
void DgBitmapSetDepthBuffer(DgBitmap *this, bool enable);
void DgBitmapDrawPixel(DgBitmap *this, uint16_t x, uint16_t y, DgColour colour);
void DgBitmapGetPixel(DgBitmap * restrict this, uint16_t x, uint16_t y, DgColour * restrict colour);
void DgBitmapDrawLine(DgBitmap * restrict this, DgVec2 pa, DgVec2 pb, DgColour * restrict colour);
void DgBitmapDrawQuadraticBezier(DgBitmap * restrict this, DgVec2 p0, DgVec2 p1, DgVec2 p2, DgColour * restrict colour);
void DgBitmapDrawPoint(DgBitmap * restrict this, float x, float y, float r, DgColour colour);
void DgBitmapDrawPoint2(DgBitmap * restrict this, DgVec2 pos, float r, DgColour *colour);
void DgBitmapDrawConvexPolygon(DgBitmap * restrict this, size_t points_count, DgVec2 * restrict points, DgColour * restrict colour);
void DgBitmapDrawTriangles(DgBitmap * restrict this, size_t count, DgBitmapTriangle * restrict triangles);
void DgBitmapDrawTrianglesIndexed(DgBitmap * restrict this, size_t count, DgBitmapIndex * restrict indexes, DgBitmapVertex * restrict vertexes);
void DgBitmapFill(DgBitmap * restrict this, DgColour colour);
void DgBitmapWritePPM(DgBitmap *this, const char * const filepath);
