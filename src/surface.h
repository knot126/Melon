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
 * Bèzier surface object
 */

#pragma once

#if defined(DG_MELON_OLD_SURFACE)
#include "maths.h"

typedef struct DgSurface3D {
	DgVec3 *points;
	uint32_t n, m;
} DgSurface3D;

DgSurface3D *DgSurface3DInit(DgSurface3D * const restrict this, uint32_t rows, uint32_t cols);
void DgSurface3DFree(DgSurface3D * restrict this);

DgVec3 DgSurface3DGetPoint(const DgSurface3D * const restrict this, uint32_t x, uint32_t y);
void DgSurface3DSetPoint(DgSurface3D * const restrict this, uint32_t x, uint32_t y, const DgVec3 * restrict value);
DgVec3 DgSurface3DGetSample(const DgSurface3D * const restrict this, float u, float v);

DgVec2I DgSurface3DGetOrder(const DgSurface3D * const restrict this);

void DgSurface3DGetBoundingBox(const DgSurface3D * const restrict this, DgVec3 * const restrict min, DgVec3 * const restrict max);
DgVec3 DgSurface3DGetBoundingBoxSize(const DgSurface3D * const restrict this);
#endif
