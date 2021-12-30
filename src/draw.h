/**
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * High-level drawing API
 */

#pragma once

#ifdef DG_ENABLE_DRAW

#include <inttypes.h>

#include "maths.h"
#include "surface.h"

/**
 * @enum DgDrawBackendType The type of graphics backend to use, like Vulkan or Software Rendering.
 */
enum {
	DG_DRAW_SOFTWARE = 1,
	DG_DRAW_VULKAN = 2,
	DG_DRAW_OPENGL = 3,
};

typedef uint32_t DgDrawBackendType;

/**
 * 2D Vertex
 * =========
 * 
 * A 2D vertex used for drawing objects.
 */
typedef struct DgVertex2D {
	DgVec2 position;
	DgVec4 colour;
} DgVertex2D;

/**
 * 3D Vertex
 * =========
 * 
 * A 3D vertex used for drawing objects.
 */
typedef struct DgVertex3D {
	DgVec3 position;
	DgVec3 colour;
} DgVertex3D;

/**
 * Drawing Context
 * ===============
 * 
 * This is the context (nonlocal state) used to draw an image. It is usually
 * used to draw to the screen instead of generate textures offline like
 * DgBitmap, though it might have a bitmap assocaited with it which it draws
 * to.
 */
typedef struct DgDrawContext {
	DgDrawBackendType backend;
	uint64_t flags;
	float surface_quality;
	bool surface_perpixel;
} DgDrawContext;

/// @typedef This establishes the DgDraw object as a handle instead of a pointer.
typedef DgDrawContext *DgDraw;

DgDraw DgDrawInitGL(DgDraw this);
DgDraw DgDrawInitVulkan(DgDraw this);
DgDraw DgDrawInitSW(DgDraw this);

uint32_t DgDrawBegin(DgDraw this, void *next);
uint32_t DgDrawEnd(DgDraw this);

uint32_t DgDrawClear(DgDraw this, DgVec4 *colour);
uint32_t DgDrawTriangles3D(DgDraw this, size_t count, DgVertex3D *vertexes);
uint32_t DgDrawSurfaces3D(DgDraw this, size_t count, DgSurface3D *surfaces);
uint32_t DgDrawTriangles2D(DgDraw this, size_t count, DgVertex2D *vertexes);
uint32_t DgDrawTexture2D(DgDraw this, DgBitmap *bitmap);

uint32_t DgDrawSurfaceMode(DgDraw this, bool perpixel, float quality);

uint32_t DgDrawFree(DgDraw this);

#endif
