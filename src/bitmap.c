/**
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * Bitmaps and Other Images
 * 
 * @todo This should be rewritten to a large extent
 */ 

#include <inttypes.h>
#include <string.h>
#include <math.h>

#include "rand.h"
#include "alloc.h"
#include "maths.h"
#include "log.h"
#include "fs.h"

#include "bitmap.h"

enum {
	DG_BITMAP_DEFAULT_CHANNELS_COUNT = 3, // For some random generation functions
};

bool DgBitmapInit(DgBitmap *bitmap, const uint16_t width, const uint16_t height, const uint16_t chan) {
	/**
	 * Initialise a bitmap that has already been allocated. Returns 1 on success
	 * or 0 on failure to allocate memory.
	 * 
	 * @param bitmap Bitmap to initialise
	 * @param width Width of the bitmap
	 * @param height Height of the bitmap
	 * @param chan Number of channels in the new bitmap
	 * @return False on success, true on failure
	 * 
	 * @todo Inconsistent naming, use DgBitmapInit
	 */
	
	memset(bitmap, 0, sizeof *bitmap);
	
	bitmap->width = width;
	bitmap->height = height;
	bitmap->chan = chan;
	
	size_t alloc_sz = width * height * chan;
	
	bitmap->src = DgAlloc(alloc_sz * sizeof *bitmap->src);
	
	if (!bitmap->src) {
		DgLog(DG_LOG_ERROR, "Failed to allocate 0x%X bytes of memory for bitmap.", alloc_sz);
		return true;
	}
	
	return false;
}

bool DgBitmapNew(DgBitmap *bitmap, const uint16_t width, const uint16_t height, const uint16_t chan) {
	return DgBitmapInit(bitmap, width, height, chan);
}

void DgBitmapFree(DgBitmap *bitmap) {
	/**
	 * Free a bitmap and all of its data.
	 * 
	 * @param bitmap Bitmap to free
	 */
	
	if (bitmap->src) {
		DgFree(bitmap->src);
	}
}

static void DgBitmapSwapSimilar(DgBitmap *dest, DgBitmap *from) {
	/**
	 * Move an atrribute-identical bitmap to where another bitmap is.
	 * 
	 * @param dest Bitmap to copy into
	 * @param src Bitmap to copy from
	 */
	
	if (dest->src) {
		DgFree(dest->src);
	}
	
	dest->src = from->src;
	from->src = NULL;
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
	
	if ((this->flags & DG_BITMAP_DRAWING_ALPHA) == DG_BITMAP_DRAWING_ALPHA) {
		DgBitmapGetPixel(this, x, y, &old_colour);
	}
	else {
		old_colour = (DgColour) {1.0f, 1.0f, 1.0f, 1.0f};
	}
	
	// Invert for graph-like coordinates
	y = (this->height - y) - 1;
	
	// Calculate pixel offset in memony block
	size_t pixel_offset = ((y * this->width) + x) * this->chan;
	
	#define DG_BITMAP_BLEND(CH) (uint8_t)(((colour.a * colour.CH) + ((1.0f - colour.a) * old_colour.CH)) * 255.0f)
	
	this->src[pixel_offset + 0] = DG_BITMAP_BLEND(r);
	if (this->chan >= 2) {
		this->src[pixel_offset + 1] = DG_BITMAP_BLEND(g);
		if (this->chan >= 3) {
			this->src[pixel_offset + 2] = DG_BITMAP_BLEND(b);
			if (this->chan >= 4) {
				this->src[pixel_offset + 3] = (uint8_t) ((old_colour.a + colour.a) * 255.0f);
			}
		}
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
	
	if (this->depth && index < (this->width * this->height) && z <= this->depth[index] && z >= 0.0f) {
		DgBitmapDrawPixel(this, x, y, *colour);
		this->depth[index] = z;
	}
	else if (this->depth == NULL) {
		DgBitmapDrawPixel(this, x, y, *colour);
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
	
	colour->r = (((float)this->src[pixel_offset + 0]) * (1.0f/255.0f));
	if (this->chan >= 2) {
		colour->g = (((float)this->src[pixel_offset + 1]) * (1.0f/255.0f));
		if (this->chan >= 3) {
			colour->b = (((float)this->src[pixel_offset + 2]) * (1.0f/255.0f));
			if (this->chan >= 4) {
				colour->a = (((float)this->src[pixel_offset + 3]) * (1.0f/255.0f));
			}
		}
	}
}

void DgBitmapDrawLine(DgBitmap * restrict this, DgVec2 pa, DgVec2 pb, DgColour *colour) {
	/**
	 * Draw a line from a point to another point.
	 * 
	 * @note This uses a variant of Bresenham's Line Algorithm. It is an
	 * incremental error algorithm, which is actually a lot more helpful to know
	 * compared to seeing an actual implementation. Basically, this means we
	 * increment x constantly (every time) but only increment y when we have
	 * collected enough "error" so that it would actually matter if we increment
	 * y. Basically, this is:
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
	 * @warning The wikipedia example is pretty misleading, you don't even need
	 * to use multiplication. Not sure why it has 1/2 as a constant either ? But
	 * some other places use that too, maybe its a draw convention issue?
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
			
			// Plot pixel for this x value
			DgBitmapDrawPixel(this, x, y, *colour);
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
			
			DgBitmapDrawPixel(this, x, y, *colour);
		}
	}
}

static inline uint16_t DgBitmapDrawQuadraticBezier_Eval(float t, float p0, float p1, float p2) {
	return (uint16_t)(((1.0f - t) * (1.0f - t) * p0) + (2.0f * (1.0f - t) * t * p1) + (t * t * p2));
}

static inline DgVec2 DgBitmapDrawQuadraticBezier_Roots(float a, float b, float c) {
	DgVec2 roots;
	
	float det = sqrtf((b * b) - (4.0f * a * c));
	
	roots.x = (-b - det) / (2.0f * a);
	roots.y = (-b + det) / (2.0f * a);
	
	return roots;
}

void DgBitmapDrawQuadraticBezier(DgBitmap * restrict this, DgVec2 p0, DgVec2 p1, DgVec2 p2, DgColour * restrict colour) {
	/**
	 * Draw a quadratic bezier curve using a naïve algorithm, so it should
	 * always work without a need for special cases.
	 * 
	 * Essentially, we use the fact that we can find a t-value from any given
	 * x-value of a curve in order to draw the curve. If you are concerned about
	 * the root finding not being a function, I would encourage you to look up
	 * what a multivalue function is.
	 * 
	 * @param this Bitmap to draw on
	 * @param p0 First control point
	 * @param p1 Second control point
	 * @param p2 Third control point
	 * @param colour Colour of the curve
	 */
	
	// Convert coordinates to the proper range
	const float base_y = (float)(this->height);
	p0 = (DgVec2) {p0.x * this->width, base_y - p0.y * base_y};
	p1 = (DgVec2) {p1.x * this->width, base_y - p1.y * base_y};
	p2 = (DgVec2) {p2.x * this->width, base_y - p2.y * base_y};
	
	// Find min and max x-values for point
	const uint16_t min_x = DgFloatMin3(p0.x, p1.x, p2.x), max_x = DgFloatMax3(p0.x, p1.x, p2.x);
	const uint16_t min_y = DgFloatMin3(p0.y, p1.y, p2.y), max_y = DgFloatMax3(p0.y, p1.y, p2.y);
	
	{
		// Precompute a and b values for x
		float a = p0.x - 2.0f * p1.x + p2.x;
		float b = 2.0f * (p1.x - p0.x);
		DgVec2 t;
		
		// Loop over each pixel with the curve
		for (size_t x = min_x; x <= max_x; x++) {
			// Find t-value(s) for the next x so we know where to stop drawing
			DgVec2 t = DgBitmapDrawQuadraticBezier_Roots(a, b, p0.x - x);
			
			// Evaluate y value(s) at this point
			// Don't need to worry about NaN for square roots
			if (0.0f <= t.data[0] && t.data[0] <= 1.0f) {
				int16_t y = DgBitmapDrawQuadraticBezier_Eval(t.data[0], p0.y, p1.y, p2.y);
				DgBitmapDrawPixel(this, x, y, *colour);
			}
			
			if (0.0f <= t.data[1] && t.data[1] <= 1.0f) {
				int16_t y = DgBitmapDrawQuadraticBezier_Eval(t.data[1], p0.y, p1.y, p2.y);
				DgBitmapDrawPixel(this, x, y, *colour);
			}
		}
	}
	
	{
		// Precompute a and b values for y
		float a = p0.y - 2.0f * p1.y + p2.y;
		float b = 2.0f * (p1.y - p0.y);
		DgVec2 t;
		
		for (size_t y = min_y; y <= max_y; y++) {
			// Find t-value(s) for the next x so we know where to stop drawing
			DgVec2 t = DgBitmapDrawQuadraticBezier_Roots(a, b, p0.y - y);
			
			// Evaluate x value(s) at this point
			// Don't need to worry about NaN for square roots
			if (0.0f <= t.data[0] && t.data[0] <= 1.0f) {
				int16_t x = DgBitmapDrawQuadraticBezier_Eval(t.data[0], p0.y, p1.y, p2.y);
				DgBitmapDrawPixel(this, x, y, *colour);
			}
			
			if (0.0f <= t.data[1] && t.data[1] <= 1.0f) {
				int16_t x = DgBitmapDrawQuadraticBezier_Eval(t.data[1], p0.y, p1.y, p2.y);
				DgBitmapDrawPixel(this, x, y, *colour);
			}
		}
	}
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
				if (x < this->width && y < this->height) {
					DgBitmapDrawPixel(this, x, y, colour);
				}
			}
		}
	}
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
	for (size_t y = pmin.y; y <= pmax.y; y++) {
		for (size_t x = pmin.x; x <= pmax.x; x++) {
			bool draw = true;
			
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
					draw = false;
					break;
				}
			}
			
			// Draw point if it can be drawn
			if (draw) {
				DgBitmapDrawPixel(this, x, y, *colour);
			}
		}
	}
}

inline static void DgBitmapDrawTriangle(DgBitmap * const this, DgBitmapTriangle *triangle) {
	/**
	 * Draw a single shaded triangle.
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
	for (size_t y = pmin.y; y <= pmax.y; y++) {
		for (size_t x = pmin.x; x <= pmax.x; x++) {
			// Convert point to float
			DgVec2 point = (DgVec2) {(float)x / (float)this->width, (float)y / (float)this->height};
			
			// Get barycentric coordinates
			DgBary3 bary = DgVec2Bary3(p1, p2, p3, point);
			
			// Check for collision
			if ((bary.u >= 0.0f) && (bary.v >= 0.0f) && (bary.w >= 0.0f)) {
				// Calculate z at this spot
				float inv_z = bary.u * p1z_inv + bary.v * p2z_inv + bary.w * p3z_inv;
				float z = 1.0f / inv_z;
				
				// Multiply by 1/z for perspective correct interpolation
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
	
	DgFile file = DgFileOpen2(filepath, DG_FILE_STREAM_WRITE);
	
	if (!file) {
		return;
	}
	
	// Write header
	DgFileStreamWriteString(file, "P6\n");
	DgFileStreamWriteIntegerString(file, this->width);
	DgFileStreamWriteString(file, " ");
	DgFileStreamWriteIntegerString(file, this->height);
	DgFileStreamWriteString(file, "\n255\n");
	
	const size_t pixels = this->width * this->height;
	
	// Write pixels
	for (size_t i = 0; i < pixels; i++) {
		DgFileStreamWrite(file, 3, &this->src[i * this->chan]);
	}
	
	DgFileStreamClose(file);
}
