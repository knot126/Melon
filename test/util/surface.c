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

#if defined(DG_MELON_OLD_SURFACE)
#include <string.h>

#include "alloc.h"
#include "maths.h"
#include "surface_implementation.h"
#include "log.h"

#include "surface.h"

DgSurface3D *DgSurface3DInit(DgSurface3D * const restrict this, uint32_t rows, uint32_t cols) {
	/**
	 * Initialise a Bèzier surface of N by M degree or order. Returns pointer to
	 * the initialised surface on success or a zero on failure.
	 * 
	 * @param this Surface to initialise
	 * @param rows Number of rows in the surface (n part of order)
	 * @param cols Number of columns in the surface (m part of order)
	 * @return Pointer of surface
	 */
	
	memset(this, 0, sizeof *this);
	
	this->n = rows;
	this->m = cols;
	
	// NOTE: Remember, order in n or m + 1 = number of rows or columns
	this->points = DgAlloc(rows * cols * sizeof *this->points);
	
	if (!this->points) {
		return NULL;
	}
	
	return this;
}

void DgSurface3DFree(DgSurface3D * restrict this) {
	/**
	 * Free resources assocaited with a surface.
	 * 
	 * @param this Surface object
	 */
	
	DgFree(this->points);
}

DgVec3 DgSurface3DGetPoint(const DgSurface3D * const restrict this, uint32_t x, uint32_t y) {
	/**
	 * Get a control point.
	 * 
	 * @param this Surface object
	 * @param x n-index of control point
	 * @param y m-index of control point
	 * @return Point on the surface
	 */
	
	return this->points[(y * this->n) + x];
}

void DgSurface3DSetPoint(DgSurface3D * const restrict this, uint32_t x, uint32_t y, const DgVec3 * restrict value) {
	/**
	 * Set a control point.
	 * 
	 * @param this Surface object
	 * @param x n-index of control point
	 * @param y m-index of control point
	 * @param value Value to set the control point to
	 */
	
	this->points[(y * this->n) + x] = *value;
}

DgVec3 DgSurface3DGetSample(const DgSurface3D * const restrict this, float u, float v) {
	/**
	 * Get a sample along the bezier surface.
	 * 
	 * @param this Surface to get sample of
	 * @param u u-coordinate of surface to get
	 * @param v v-coordinate of surface to get
	 * @return Sampled point on the surface
	 */
	
	if (u > 1.0f) {
		u = 1.0f;
	}
	
	if (v > 1.0f) {
		v = 1.0f;
	}
	
	if (u < 0.0f) {
		u = 0.0f;
	}
	
	if (v < 0.0f) {
		v = 0.0f;
	}
	
	const DgVec3 p = DgBezSurfVec3(this->n, this->m, this->points, u, v);
	
	return p;
}

DgVec2I DgSurface3DGetOrder(const DgSurface3D * const restrict this) {
	/**
	 * Get the order of a surface.
	 * 
	 * @param this Surface object
	 * @return Order of the surface, where x = n and y = m
	 */
	
	return (DgVec2I) {this->n, this->m};
}

void DgSurface3DGetBoundingBox(const DgSurface3D * const restrict this, DgVec3 * const restrict min, DgVec3 * const restrict max) {
	/**
	 * Get the bounding box of the surface.
	 * 
	 * @param this Surface object
	 * @param min Vector to write the minium point in the surface to
	 * @param max Vector to write the maxium point in the surface to
	 */
	
	*min = (DgVec3) {100000000000000.0f, 100000000000000.0f, 100000000000000.0f};
	*max = (DgVec3) {-100000000000000.0f, -100000000000000.0f, -100000000000000.0f};
	
	if (!this) {
		return;
	}
	
	for (size_t x = 0; x < this->n; x++) {
		for (size_t y = 0; y < this->m; y++) {
			DgVec3 cur = DgSurface3DGetPoint(this, x, y);
			
			if (cur.x >= max->x) {
				max->x = cur.x;
			}
			
			if (cur.y >= max->y) {
				max->y = cur.y;
			}
			
			if (cur.z >= max->z) {
				max->z = cur.z;
			}
			
			if (cur.x <= min->x) {
				min->x = cur.x;
			}
			
			if (cur.y <= min->y) {
				min->y = cur.y;
			}
			
			if (cur.z <= min->z) {
				min->z = cur.z;
			}
		}
	}
}

DgVec3 DgSurface3DGetBoundingBoxSize(const DgSurface3D * const restrict this) {
	/**
	 * Get the size of the bounding box containing control points.
	 * 
	 * @param this Surface object
	 * @return Size of the bounding box of the surface
	 */
	
	if (!this) {
		return (DgVec3) {0.0f, 0.0f, 0.0f};
	}
	
	DgVec3 min, max;
	
	DgSurface3DGetBoundingBox(this, &min, &max);
	
	return DgVec3Subtract(max, min);
}
#endif
