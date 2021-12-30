/**
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * High-level drawing API
 */ 

#ifdef DG_ENABLE_DRAW

#include "alloc.h"
#include "str.h"

#include "draw.h"

inline static DgDraw DgDrawCreateIfNull(DgDraw this) {
	/**
	 * Allocate DgDraw memory if pointer is null.
	 */
	
	if (!this) {
		this = DgAlloc(sizeof *this);
		
		if (!this) {
			return NULL;
		}
	}
	
	memset(this, 0, sizeof *this);
	
	return this;
}

DgDraw DgDrawInitGL(DgDraw this) {
	/**
	 * Initialise the drawing context with an OpenGL instance.
	 * 
	 * @param this Optional pointer the the memory to use
	 */
	
	return NULL;
}

DgDraw DgDrawInitVulkan(DgDraw this) {
	/**
	 * Initialise the drawing context using Vulkan as a backend.
	 * 
	 * @param this Optional pointer the the memory to use
	 */
	
	return NULL;
}

DgDraw DgDrawInitSW(DgDraw this) {
	/**
	 * Initialise the drawing context with an OpenGL instance.
	 * 
	 * @param this Optional pointer the the memory to use
	 */
	
	this = DgDrawCreateIfNull(this);
	
	if (!this) {
		return NULL;
	}
	
	this->backend = DG_DRAW_SOFTWARE;
	
	return this;
}

uint32_t DgDrawBegin(DgDraw this, void *next) {
	/**
	 * Initialises the new frame and clears the previous frame's data.
	 * 
	 * @param this Drawing context
	 * @param next Extra paramater for later extension
	 */
	
	
}

uint32_t DgDrawEnd(DgDraw this) {
	/**
	 * Finishes the current frame and displays it to the screen.
	 * 
	 * @param this Drawing context
	 */
	
	
}

uint32_t DgDrawClear(DgDraw this, DgVec4 *colour) {
	/**
	 * "Clears" the current frame. More accurately, this sets the background colour.
	 * 
	 * @param colour Background colour to use
	 */
	
	
}

uint32_t DgDrawTriangles3D(DgDraw this, size_t count, DgVertex3D *vertexes) {
	/**
	 * Display a group of vertexes to the screen.
	 * 
	 * @param this Drawing context
	 * @param count Number of verticies to draw to the screen
	 * @param vertexes Pointer to the verticies to draw
	 */
	
	
}

uint32_t DgDrawSurfaces3D(DgDraw this, size_t count, DgSurface3D *surfaces) {
	/**
	 * Draw a group of surfaces
	 * 
	 * @param this Drawing context
	 * @param count Number of surfaces to display
	 * @param surfaces Pointer to the surfaces to draw
	 */
	
	
}

uint32_t DgDrawTriangles2D(DgDraw this, size_t count, DgVertex2D *vertexes) {
	/**
	 * Draw triangles in 2D
	 * 
	 * @param this Drawing context
	 * @param count Number of triangles to draw
	 * @param vertexes Verticies to draw
	 */
	
	
}

uint32_t DgDrawTexture2D(DgDraw this, DgBitmap *bitmap) {
	/**
	 * Draws a 2D texture to the screen; that is, displays it over the current
	 * drawing.
	 * 
	 * @param this Drawing context
	 * @param bitmap Image to draw over the frame 
	 */
	
	
}

uint32_t DgDrawSurfaceMode(DgDraw this, bool perpixel, float quality) {
	/**
	 * Sets the mode that surfaces will draw with.
	 * 
	 * @param perpixel The surfaces will be drawn using a per-pixel algorithm.
	 * @param quality If not using per-pixel, then this is the quality of surface renderings.
	 */
	
	this->surface_perpixel = perpixel;
	this->surface_quality = quality;
	
	return 0;
}

uint32_t DgDrawFree(DgDraw this) {
	/**
	 * Destroy a drawing context. This does not free memory if NULL was passed
	 * to DgDrawInit
	 * 
	 * @param this Drawing context to destroy
	 */
	
	
}

#endif
