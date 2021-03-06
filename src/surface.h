/*
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * Bèzier surface object
 */

#pragma once

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
