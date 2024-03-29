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
 * 
 * @todo This should be rewritten to a large extent
 */ 

#include <inttypes.h>
#include <string.h>
#include <math.h>

#include "alloc.h"
#include "maths.h"
#include "log.h"
#include "storage.h"
#include "error.h"

#include "bitmap.h"

DgError DgBitmapInit(DgBitmap *bitmap, DgVec2I size, const uint16_t chan) {
	/**
	 * Initialise a bitmap that has already been allocated. Returns 1 on success
	 * or 0 on failure to allocate memory.
	 * 
	 * @param bitmap Bitmap to initialise
	 * @param size Width and height of the bitmap
	 * @param chan Number of channels in the new bitmap
	 * @return DG_ERROR_ALLOCATION_FAILED when the bitmap fails to allocate
	 *         DG_ERROR_SUCCESS when the bitmap is created successfully
	 * 
	 * @todo Inconsistent naming, use DgBitmapInit
	 */
	
	memset(bitmap, 0, sizeof *bitmap);
	
	bitmap->width = size.x;
	bitmap->height = size.y;
	bitmap->chan = chan;
	
	size_t alloc_sz = size.x * size.y * chan;
	
	bitmap->src = DgAlloc(alloc_sz * sizeof *bitmap->src);
	
	if (!bitmap->src) {
		return DG_ERROR_ALLOCATION_FAILED;
	}
	
	return DG_ERROR_SUCCESS;
}

void DgBitmapFree(DgBitmap *bitmap) {
	/**
	 * Free a bitmap and all of its data.
	 * 
	 * @param bitmap Bitmap to free
	 */
	
	if (bitmap->src && !(bitmap->flags & DG_BITMAP_EXTERNAL_SOURCE)) {
		DgFree(bitmap->src);
	}
	
	if (bitmap->depth) {
		DgFree(bitmap->depth);
	}
}

void DgBitmapSetSource(DgBitmap * restrict this, uint8_t * restrict source, DgVec2I size, uint16_t channels) {
	/**
	 * Set the source to be external memory which isn't managed by DgBitmap.
	 * 
	 * @param this Bitmap object to use
	 * @param source Source block of memory to use
	 * @param size Size of the bitmap
	 * @param channels Number of channels in the bitmap
	 */
	
	if (this->src && !(this->flags & DG_BITMAP_EXTERNAL_SOURCE)) {
		DgFree(this->src);
	}
	
	this->src = source;
	
	this->width = size.x;
	this->height = size.y;
	this->chan = channels;
	
	DgBitmapSetFlags(this, DgBitmapGetFlags(this) | DG_BITMAP_EXTERNAL_SOURCE);
}

static DgVec2I DgBitmapToScreenSpace(DgBitmap *this, DgVec2 point) {
	/**
	 * Convert graph (resolution-independent) coordinates to screen space
	 * coordinates.
	 * 
	 * @param this Bitmap object
	 * @param point Graph coordinates
	 * @return Coordinates in screen space
	 */
	
	DgVec2I screen;
	
	screen.x = (uint32_t)((point.x * (float)(this->width)));
	screen.y = (uint32_t)((point.y * (float)(this->height)));
	
	screen.y = this->height - screen.y - 1;
	
	return screen;
}

void DgBitmapSetFlags(DgBitmap *this, DgBitmapFlags flags) {
	/**
	 * Set the flags on a bitmap.
	 * 
	 * @param this Bitmap to set flags on
	 * @param flags The flags that should be set
	 */
	
	this->flags = flags;
}

DgBitmapFlags DgBitmapGetFlags(DgBitmap *this) {
	/**
	 * Get the flags on a bitmap.
	 * 
	 * @param this Bitmap to get flags from
	 * @return The flags on the bitmap
	 */
	
	return this->flags;
}

void DgBitmapSetDepthBuffer(DgBitmap *this, bool enable) {
	/**
	 * Enable or disable the depth buffer.
	 * 
	 * @param this Bitmap object
	 * @param enable Enable (true) or disable (false) the depth buffer
	 */
	
	// If depth buffer is not present and we want to enable
	if (enable && !this->depth) {
		// Allocate memory for the depth buffer
		this->depth = DgAlloc(sizeof *this->depth * this->width * this->height);
		
		// Fill the depth buffer with default values
		for (size_t i = 0; i < this->width * this->height; i++) {
			this->depth[i] = INFINITY;
		}
	}
	// If depth buffer is present and we want to disable
	else if (!enable && this->depth) {
		DgFree(this->depth);
	}
	// Otherwise nothing is needed
}

void DgBitmapDrawPixel(DgBitmap *this, uint16_t x, uint16_t y, DgColour colour) {
	/**
	 * Draw a single pixel to a bitmap.
	 * 
	 * @param this Bitmap to draw pixel on
	 * @param x The x-location of the pixel
	 * @param y The y-location of the pixel
	 * @param colour The colour of the pixel
	 */
	
	if (x >= this->width || y >= this->height) {
		return;
	}
	
	// Old colour for alpha blending
	DgColour old_colour;
	DgBitmapGetPixel(this, x, y, &old_colour);
	
	// Invert for graph-like coordinates
	y = (this->height - y) - 1;
	
	// Calculate pixel location in memory block
	size_t pixel_offset = ((y * this->width) + x) * this->chan;
	
	// Blend colours
	#define DG_BITMAP_BLEND(CH) (uint8_t)(((colour.a * colour.CH) + ((1.0f - colour.a) * old_colour.CH)) * 255.0f)
	
	this->src[pixel_offset + 0] = DG_BITMAP_BLEND(r);
	if (this->chan >= 2) {
		this->src[pixel_offset + 1] = DG_BITMAP_BLEND(g);
		if (this->chan >= 3) {
			this->src[pixel_offset + 2] = DG_BITMAP_BLEND(b);
			if (this->chan >= 4) {
				this->src[pixel_offset + 3] = 0xff;
			}
		}
	}
	
	// I don't know why I should have to implement this but of course windows
	// does fucking BGR colour.
	if (this->chan >= 3 && (this->flags & DG_BITMAP_UNFUCK_RGB)) {
		uint8_t t = this->src[pixel_offset + 0];
		this->src[pixel_offset + 0] = this->src[pixel_offset + 2];
		this->src[pixel_offset + 2] = t;
	}
	
	#undef DG_BITMAP_BLEND
}

void DgBitmapDrawPixelZ(DgBitmap * restrict this, uint16_t x, uint16_t y, float z, DgColour * restrict colour) {
	/**
	 * Draw a single pixel to a bitmap.
	 * 
	 * @param this Bitmap to draw pixel on
	 * @param x The x-location of the pixel
	 * @param y The y-location of the pixel
	 * @param z The z-location (depth) of the pixel
	 * @param colour The colour of the pixel
	 */
	
	size_t index = ((this->width * y) + x);
	
	DgBitmapDrawPixel(this, x, y, *colour);
	
	if ((this->depth != NULL) && (index < (this->width * this->height)) && (z <= this->depth[index]) && (z >= 0.0f)) {
		this->depth[index] = z;
	}
}

void DgBitmapGetPixel(DgBitmap * restrict this, uint16_t x, uint16_t y, DgColour * restrict colour) {
	/**
	 * Get a pixel from a bitmap.
	 * 
	 * @param this Bitmap to retrive pixel from
	 * @param x The x coordinate to retrive from
	 * @param y The y coordinate to retrive from
	 * @param colour Pointer to vec4 to write colour to
	 */
	
	x %= this->width;
	y %= this->height;
	
	// Invert for graph-like coordinates
	y = (this->height - y) - 1;
	
	// Calculate pixel offset in memory block
	size_t pixel_offset = ((y * this->width) + x) * this->chan;
	
	colour->r = (((float)this->src[pixel_offset + 0]) * (1.0f / 255.0f));
	if (this->chan >= 2) {
		colour->g = (((float)this->src[pixel_offset + 1]) * (1.0f / 255.0f));
		if (this->chan >= 3) {
			colour->b = (((float)this->src[pixel_offset + 2]) * (1.0f / 255.0f));
			if (this->chan >= 4) {
				colour->a = (((float)this->src[pixel_offset + 3]) * (1.0f / 255.0f));
			}
		}
	}
}

void DgBitmapDrawLine(DgBitmap * restrict this, DgVec2 pa, DgVec2 pb, DgColour *colour) {
	/**
	 * Draw a line from a point to another point.
	 * 
	 * @note This uses something like Bresenham's Line Algorithm. It is an
	 * incremental error algorithm. Basically, this means we increment x
	 * constantly (every time) but only increment y when we have collected
	 * enough "error" so that it would actually matter if we increment y.
	 * Basically, this is:
	 * 
	 * -- line is in form y = rx + c (more commonly y = mx + b)
	 * y_error = 0
	 * 
	 * foreach x between x_start to x_end do
	 *     y_error += m -- add the change in y comapred to the change in x to the error
	 *     
	 *     if (y_error >= 1) then -- if the error of the change in y is noticable to the rasterised image (>= 1)
	 *         y += 1
	 *         y_error -= 1 -- note: we need to keep even the tiny bit of error still there, otherwise
	 *                      -- we won't be able to draw all slopes. Try changing it to y_error=0 and drawing many
	 *                      -- different slopes to see the issue that occurs!
	 *     end
	 *     
	 *     draw(x, y)
	 * end
	 * 
	 * This can be reformed to the implementation below for using only integer
	 * arithmetic and to support vertical lines by swapping x and y for r > 1.
	 * 
	 * @note Updated (2022-11-08) so that the line is now anti-aliased. This only
	 * works if DG_BITMAP_DRAWING_ALPHA is set.
	 * 
	 * @todo Make line colours with alpha work when AA is enabled.
	 * 
	 * @param this Bitmap object
	 * @param pa First point on the line
	 * @param pb Second point on the line
	 * @param colour Colour to fill the line with
	 */
	
	DgVec2I a = DgBitmapToScreenSpace(this, pa);
	DgVec2I b = DgBitmapToScreenSpace(this, pb);
	
	if (a.x > b.x) {
		DgVec2I c = b;
		b = a;
		a = c;
	}
	
	int32_t dx = b.x - a.x;
	int32_t dy = b.y - a.y;
	int32_t error = 0;
	const int32_t aa = (this->flags & DG_BITMAP_DRAWING_ALPHA) | (this->flags & DG_BITMAP_DRAWING_ANTIALIAS);
	
	// As part of anti-aliasing the line, we need a local copy of the colour
	DgColour current = *colour;
	
	// For lines with y'(x) in the range [-1, 1] use x increment
	if ((dx >= 0) ? ((-dx <= dy) && (dy < dx)) : ((-dx >= dy) && (dy >= dx))) {
		// Starting y value
		int32_t y = a.y;
		
		// Loop over each x value
		for (int32_t x = a.x; x <= b.x; x++) {
			// Add the error for this increment in y
			error += dy;
			
			// Increment y if error is significant
			if (error >= dx) {
				y += 1;
				error -= dx;
			}
			else if (error <= -dx) {
				y -= 1;
				error += dx;
			}
			
			if (aa) {
				// Find how far off the point is from this pixel as the alpha
				float offset = (float)(error) / (float)(dx);
				
				// Alpha for the main pixel
				current.a = 1.0f - fabs(offset);
				
				// Plot main pixel for this x value
				DgBitmapDrawPixel(this, x, y, current);
				
				// Depending if the offset is less or more, draw the rest of the line
				// up or down.
				if (offset >= 0.0f) {
					current.a = fabs(offset);
					DgBitmapDrawPixel(this, x, y + 1, current);
				}
				else {
					current.a = fabs(offset);
					DgBitmapDrawPixel(this, x, y - 1, current);
				}
			}
			else {
				DgBitmapDrawPixel(this, x, y, *colour);
			}
		}
	}
	// For lines outside of that range, use y increment
	else {
		// Recalculate min and max
		if (a.y > b.y) {
			DgVec2I c = b;
			b = a;
			a = c;
		}
		
		// Recalculate deltas
		dx = b.x - a.x;
		dy = b.y - a.y;
		
		int32_t x = a.x;
		
		for (int32_t y = a.y; y <= b.y; y++) {
			error += dx;
			
			if (error >= dy) {
				x += 1;
				error -= dy;
			}
			else if (error <= -dy) {
				x -= 1;
				error += dy;
			}
			
			if (aa) {
				// Find how far off the point is from this pixel as the alpha
				float offset = (float)(error) / (float)(dy);
				
				// Alpha for the main pixel
				current.a = 1.0f - fabs(offset);
				
				// Plot main pixel for this y value
				DgBitmapDrawPixel(this, x, y, current);
				
				// Depending if the offset is less or more, draw the rest of the line
				// up or down.
				if (offset >= 0.0f) {
					current.a = fabs(offset);
					DgBitmapDrawPixel(this, x + 1, y, current);
				}
				else {
					current.a = fabs(offset);
					DgBitmapDrawPixel(this, x - 1, y, current);
				}
			}
			else {
				DgBitmapDrawPixel(this, x, y, *colour);
			}
		}
	}
}

void DgBitmapDrawQuadraticBezier(DgBitmap * restrict this, DgVec2 p0, DgVec2 p1, DgVec2 p2, DgColour * restrict colour) {
	/**
	 * This didn't work and was removed.
	 * 
	 * @param this Bitmap to draw on
	 * @param p0 First control point
	 * @param p1 Second control point
	 * @param p2 Third control point
	 * @param colour Colour of the curve
	 */
	
	return;
}

void DgBitmapDrawPoint(DgBitmap * restrict this, float x, float y, float r, DgColour colour) {
	/**
	 * Draw a dot to the bitmap at the given coordinates with r radius.
	 * 
	 * @param this Bitmap to draw pixel on
	 * @param x The x-location of the pixel, within [0.0f, 1.0f]
	 * @param y The y-location of the pixel, within [0.0f, 1.0f]
	 * @param r The radius of the dot
	 * @param colour The colour to paint the pixel
	 */
	
	y = 1.0f - y;
	
	r *= (this->width < this->height) ? this->width : this->height;
	uint16_t rad = (uint16_t) r;
	uint32_t radsq = (uint32_t) r * r;
	uint16_t ix = (uint16_t) (x * (float)this->width);
	uint16_t iy = (uint16_t) (y * (float)this->height);
	
	for (int32_t y = iy - rad - 2; y < iy + rad + 2; y++) {
		for (int32_t x = ix - rad - 2; x < ix + rad + 2; x++) {
			uint32_t raddist = ((x - ix) * (x - ix)) + ((y - iy) * (y - iy));
			if (raddist <= radsq) {
				DgBitmapDrawPixel(this, x, y, colour);
			}
		}
	}
}

void DgBitmapDrawPoint2(DgBitmap * restrict this, DgVec2 pos, float r, DgColour *colour) {
	DgBitmapDrawPoint(this, pos.x, pos.y, r, *colour);
}

static bool DgBitmapIsInLineSame(DgVec2 *a, DgVec2 *b, DgVec2 *c, DgVec2 *d) {
	/**
	 * Returns true if point vector c and d are on the same side as the line
	 * formed by points a and b.
	 * 
	 * @param a Start of the line
	 * @param b End of the line
	 * @param c Point 1
	 * @param d Point 2
	 */
	
	DgVec2 line = DgVec2Subtract(*b, *a);
	
	float result_a = DgVec2RotDot(line, DgVec2Subtract(*c, *a));
	float result_b = DgVec2RotDot(line, DgVec2Subtract(*d, *a));
	
	int32_t sa = DgSign(result_a);
	int32_t sb = DgSign(result_b);
	
	return (sa == sb);
}

void DgBitmapDrawConvexPolygon(DgBitmap * restrict this, size_t points_count, DgVec2 * restrict points, DgColour * restrict colour) {
	/**
	 * Draw a convex polygon given its verticies and colour.
	 * 
	 * @note This is a naïve implementation (at least I think?) and only a first
	 * attempt. It was initially my solution just to draw triangles but since it
	 * works for all shapes I have decided to implement that instead.
	 * 
	 * @param this Bitmap object to draw to
	 * @param points_count Number of points in the shape
	 * @param points Points of the convex polygon to draw
	 * @param colour The colour that the points inside the shape should be set to
	 */
	
	DgVec2 min, max;
	
	// Compute min and max coordinates
	for (size_t i = 0; i < points_count; i++) {
		if (points[i].x < min.x) {
			min.x = points[i].x;
		}
		
		if (points[i].y < min.y) {
			min.y = points[i].y;
		}
		
		if (points[i].x > max.x) {
			max.x = points[i].x;
		}
		
		if (points[i].y > max.y) {
			max.y = points[i].y;
		}
	}
	
	// Calculate integer min/max
	DgVec2I pmin, pmax;
	
	pmin.x = (uint32_t)(min.x * (float)(this->width));
	pmin.y = (uint32_t)(min.y * (float)(this->height));
	pmax.x = (uint32_t)(max.x * (float)(this->width));
	pmax.y = (uint32_t)(max.y * (float)(this->height));
	
	// Get the "centre" of the shape; this should always work for convex shapes
	DgVec2 centre;
	
	for (size_t i = 0; i < points_count; i++) {
		centre = DgVec2Add(centre, points[i]);
	}
	
	centre = DgVec2Scale(1.0f / (float)points_count, centre);
	
	// Fill in the pixels
	for (int32_t y = pmin.y; y <= pmax.y; y++) {
		for (int32_t x = pmin.x; x <= pmax.x; x++) {
			bool alpha = 1.0f;
			
			// Point coordinates
			DgVec2 pc = (DgVec2) {(float)x / (float)this->width, (float)y / (float)this->height};
			
			// For each point
			for (size_t i = 0; i < points_count; i++) {
				// Line coordinates
				DgVec2 pa = points[i], pb = points[(i + 1) % points_count];
				
				// Test point
				bool result = DgBitmapIsInLineSame(&pa, &pb, &pc, &centre);
				
				// Check if on the right side of the line
				if (!result) {
					alpha = 0.0f;
					break;
				}
			}
			
			// Draw point if it should be drawn
			if (alpha > 0.0f) {
				DgColour final_colour = *colour;
				final_colour.a *= alpha;
				DgBitmapDrawPixel(this, x, y, final_colour);
			}
		}
	}
}

inline static void DgBitmapDrawTriangle(DgBitmap * const this, DgBitmapTriangle *triangle) {
	/**
	 * Draw a single shaded triangle.
	 * 
	 * @warning Kind of a messy function
	 * 
	 * @note This is not currenlty a fast implementation
	 * 
	 * @param this Bitmap object
	 * @param triangle Triangle to draw
	 */
	
	// Negate Z coordinate if needed
	if ((this->flags & DG_BITMAP_DRAWING_NEGATE_Z) == DG_BITMAP_DRAWING_NEGATE_Z) {
		triangle->p1.position.z = -triangle->p1.position.z;
		triangle->p2.position.z = -triangle->p2.position.z;
		triangle->p3.position.z = -triangle->p3.position.z;
	}
	
	// Find 2D coordinates on screen
	DgVec2 p1, p2, p3;
	
	if ((this->flags & DG_BITMAP_DRAWING_PERSPECTIVE) == DG_BITMAP_DRAWING_PERSPECTIVE) {
		// Perspective enabled
		if ((this->flags & DG_BITMAP_NO_CORRECT_COORDINATES) == DG_BITMAP_NO_CORRECT_COORDINATES) {
			p1.x = ((triangle->p1.position.x) / triangle->p1.position.z);
			p1.y = ((triangle->p1.position.y) / triangle->p1.position.z);
			p2.x = ((triangle->p2.position.x) / triangle->p2.position.z);
			p2.y = ((triangle->p2.position.y) / triangle->p2.position.z);
			p3.x = ((triangle->p3.position.x) / triangle->p3.position.z);
			p3.y = ((triangle->p3.position.y) / triangle->p3.position.z);
		}
		else {
			p1.x = (((triangle->p1.position.x) / triangle->p1.position.z) * 0.5f) + 0.5f;
			p1.y = (((triangle->p1.position.y) / triangle->p1.position.z) * 0.5f) + 0.5f;
			p2.x = (((triangle->p2.position.x) / triangle->p2.position.z) * 0.5f) + 0.5f;
			p2.y = (((triangle->p2.position.y) / triangle->p2.position.z) * 0.5f) + 0.5f;
			p3.x = (((triangle->p3.position.x) / triangle->p3.position.z) * 0.5f) + 0.5f;
			p3.y = (((triangle->p3.position.y) / triangle->p3.position.z) * 0.5f) + 0.5f;
		}
	}
	else {
		// Perspective disabled
		p1.x = triangle->p1.position.x;
		p1.y = triangle->p1.position.y;
		p2.x = triangle->p2.position.x;
		p2.y = triangle->p2.position.y;
		p3.x = triangle->p3.position.x;
		p3.y = triangle->p3.position.y;
	}
	
	// Compute min and max coordinates
	DgVec2 min, max;
	
	min.x = DgFloatMin3(p1.x, p2.x, p3.x);
	min.y = DgFloatMin3(p1.y, p2.y, p3.y);
	max.x = DgFloatMax3(p1.x, p2.x, p3.x);
	max.y = DgFloatMax3(p1.y, p2.y, p3.y);
	
	// Calculate integer min/max
	DgVec2I pmin, pmax;
	
	pmin.x = (uint32_t)(min.x * (float)(this->width));
	pmin.y = (uint32_t)(min.y * (float)(this->height));
	pmax.x = (uint32_t)(max.x * (float)(this->width));
	pmax.y = (uint32_t)(max.y * (float)(this->height));
	
	// Precompute inverse z
	float p1z_inv = 1.0f / triangle->p1.position.z;
	float p2z_inv = 1.0f / triangle->p2.position.z;
	float p3z_inv = 1.0f / triangle->p3.position.z;
	
	// Fill in the pixels
	for (int32_t y = pmin.y; y <= pmax.y; y++) {
		for (int32_t x = pmin.x; x <= pmax.x; x++) {
			// Convert point to float
			DgVec2 point = (DgVec2) {(float)x / (float)this->width, (float)y / (float)this->height};
			
			// Get barycentric coordinates
			DgBary3 bary = DgVec2Bary3(p1, p2, p3, point);
			
			// Check for collision
			if ((bary.u >= 0.0f) && (bary.v >= 0.0f) && (bary.w >= 0.0f)) {
				// Interpolate to get z at this spot
				// $$ \frac{1}{(uP_{1_z}^{-1} + vP_{2_z}^{-1} + wP_{3_z}^{-1})} $$
				float inv_z = bary.u * p1z_inv + bary.v * p2z_inv + bary.w * p3z_inv;
				float z = 1.0f / inv_z;
				
				// Multiply by 1/z for perspective correct interpolation
				// $$ \frac{z}{(z^{-1}\vec{C}_1 + z^{-1}\vec{C}_2 + z^{-1}\vec{C}_3)} $$
				DgVec4 pa = DgVec4Scale(inv_z, triangle->p1.colour);
				DgVec4 pb = DgVec4Scale(inv_z, triangle->p2.colour);
				DgVec4 pc = DgVec4Scale(inv_z, triangle->p3.colour);
				
				// Evaluate pixel colour based on barycentric coordinates
				DgColour c = DgVec4Scale(z, DgVec4Bary3Evaluate(bary.u, &pa, bary.v, &pb, bary.w, &pc));
				
				// Draw the pixel
				// Interplation should be replaced with perspective correct variant
				DgBitmapDrawPixelZ(this, x, y, z, &c);
			}
		}
	}
}

void DgBitmapDrawTriangles(DgBitmap * restrict this, size_t count, DgBitmapTriangle * restrict triangles) {
	/**
	 * Draw multipule shaded triangles.
	 * 
	 * @param this Bitmap object to draw to
	 * @param count Number of triangles to draw
	 * @param triangles Array of triangles to draw
	 */
	
	for (size_t t = 0; t < count; t++) {
		DgBitmapDrawTriangle(this, &triangles[t]);
	}
}

void DgBitmapDrawTrianglesIndexed(DgBitmap * restrict this, size_t count, DgBitmapIndex * restrict indexes, DgBitmapVertex * restrict vertexes) {
	/**
	 * Draw multipule shaded triangles using indexes.
	 * 
	 * @param this Bitmap object
	 * @param count Number of indexes to draw
	 * @param indexes Indexes of triangle vertexes
	 * @param vertexes Points on the triangles
	 */
	
	DgBitmapTriangle triangle;
	
	for (size_t i = 0; i < count; i++) {
		triangle.data[0] = vertexes[indexes[i].data[0]];
		triangle.data[1] = vertexes[indexes[i].data[1]];
		triangle.data[2] = vertexes[indexes[i].data[2]];
		DgBitmapDrawTriangle(this, &triangle);
	}
}

void DgBitmapFill(DgBitmap * restrict this, DgColour colour) {
	/**
	 * Fill a bitmap with a given colour.
	 * 
	 * @param this Bitmap to draw to
	 * @param colour The colour to fill the image with
	 */
	
	for (size_t y = 0; y < this->height; y++) {
		for (size_t x = 0; x < this->width; x++) {
			DgBitmapDrawPixel(this, x, y, colour);
		}
	}
	
	if (this->depth != NULL) {
		for (size_t i = 0; i < this->width * this->height; i++) {
			this->depth[i] = INFINITY;
		}
	}
}

void DgBitmapWritePPM(DgBitmap *this, const char * const filepath) {
	/**
	 * Write the bitmap to a P6 PPM file, if the image has more than three
	 * channels.
	 * 
	 * @param this Bitmap to write out
	 * @param filepath Path to write the file
	 */
	
	if (this->chan < 3) {
		return;
	}
	
	DgStream stream;
	DgStreamOpen(NULL, &stream, filepath, DG_STREAM_WRITE);
	
	// Write header
	DgStreamWriteString(&stream, "P6\n");
	DgStreamWriteIntegerString(&stream, this->width);
	DgStreamWriteString(&stream, " ");
	DgStreamWriteIntegerString(&stream, this->height);
	DgStreamWriteString(&stream, "\n255\n");
	
	const size_t pixels = this->width * this->height;
	
	// Write pixels
	for (size_t i = 0; i < pixels; i++) {
		DgStreamWrite(&stream, 3, &this->src[i * this->chan]);
	}
	
	DgStreamClose(&stream);
}
