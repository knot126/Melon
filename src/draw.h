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
 * High-level drawing API
 */

#pragma once

#ifdef DG_ENABLE_DRAW

#include <inttypes.h>

#include "maths.h"
#include "surface.h"
#include "error.h"

/**
 * Enums
 * =====
 * 
 * Enums for the draw module
 */
enum {
	DG_DRAW_SOFTWARE = 0,
	//DG_DRAW_COMPUTE = 1,
	//DG_DRAW_OPENGL = 2,
	//DG_DRAW_VULKAN = 3,
	//DG_DRAW_DIRECTX = 4,
	//DG_DRAW_METAL = 5,
};

/**
 * 2D Vertex
 * =========
 * 
 * A 2D vertex used for drawing objects.
 */
typedef struct DgVertex2D {
	DgVec2 position;
	DgColour colour;
} DgVertex2D;

/**
 * 3D Vertex
 * =========
 * 
 * A 3D vertex used for drawing objects.
 */
typedef struct DgVertex3D {
	DgVec3 position;
	DgColour colour;
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
typedef struct DgDraw {
	uint64_t flags : 60;
	uint64_t backend : 4;
	void *context;
	float surface_quality;
	bool surface_perpixel;
} DgDraw;



#endif
