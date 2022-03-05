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
	bitmap->alloc_sz = width * height * chan;
	bitmap->src = DgAlloc(bitmap->alloc_sz * sizeof *bitmap->src);
	
	if (!bitmap->src) {
		DgLog(DG_LOG_ERROR, "Failed to allocate 0x%X bytes of memory for bitmap.", bitmap->alloc_sz);
		return true;
	}
	
	uint8_t *src = bitmap->src;
	
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

void DgBitmapDrawPixel(DgBitmap *this, uint16_t x, uint16_t y, DgVec4 colour) {
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
	DgVec4 old_colour;
	
	if ((this->flags & DG_BITMAP_DRAWING_ALPHA) == DG_BITMAP_DRAWING_ALPHA) {
		DgBitmapGetPixel(this, x, y, &old_colour);
	}
	else {
		old_colour = (DgVec4) {1.0f, 1.0f, 1.0f, 1.0f};
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
				this->src[pixel_offset + 3] = (uint8_t) (colour.a * 255.0f);
			}
		}
	}
	
	#undef DG_BITMAP_BLEND
}

void DgBitmapGetPixel(DgBitmap * restrict this, uint16_t x, uint16_t y, DgVec4 * restrict colour) {
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

void DgBitmapDrawPoint(DgBitmap * restrict this, float x, float y, float r, DgVec4 colour) {
	/**
	 * Draw a dot to the bitmap at the given coordinates with r radius.
	 * 
	 * @param this Bitmap to draw pixel on
	 * @param x The x-location of the pixel, within [0.0f, 1.0f]
	 * @param y The y-location of the pixel, within [0.0f, 1.0f]
	 * @param r The radius of the dot
	 * @param colour The colour to paint the pixel
	 */
	
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

void DgBitmapDrawConvexPolygon(DgBitmap * restrict this, size_t points_count, DgVec2 * restrict points, DgVec4 * restrict colour) {
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

void DgBitmapDrawTriangles(DgBitmap * restrict this, size_t count, DgBitmapTriangle * restrict triangles) {
	/**
	 * Draw a shaded triangle.
	 * 
	 * @param this Bitmap object to draw to
	 * @param count Number of triangles to draw
	 * @param triangles Array of triangles to draw
	 */
	
	DgVec2 min, max;
	
	for (size_t t = 0; t < count; t++) {
		DgBitmapTriangle *tri = &triangles[t];
		
		// Find 2D coordinates on screen
		DgVec2 p1, p2, p3;
		
		if ((this->flags & DG_BITMAP_DRAWING_PERSPECTIVE) == DG_BITMAP_DRAWING_PERSPECTIVE) {
			// Perspective enabled
			p1.x = tri->p1.position.x / tri->p1.position.z;
			p1.y = tri->p1.position.y / tri->p1.position.z;
			p2.x = tri->p2.position.x / tri->p2.position.z;
			p2.y = tri->p2.position.y / tri->p2.position.z;
			p3.x = tri->p3.position.x / tri->p3.position.z;
			p3.y = tri->p3.position.y / tri->p3.position.z;
		}
		else {
			// Perspective disabled
			p1.x = tri->p1.position.x;
			p1.y = tri->p1.position.y;
			p2.x = tri->p2.position.x;
			p2.y = tri->p2.position.y;
			p3.x = tri->p3.position.x;
			p3.y = tri->p3.position.y;
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
		
		// Fill in the pixels
		for (size_t y = pmin.y; y <= pmax.y; y++) {
			for (size_t x = pmin.x; x <= pmax.x; x++) {
				// Convert point to float
				DgVec2 point = (DgVec2) {(float)x / (float)this->width, (float)y / (float)this->height};
				
				//DgLog(DG_LOG_INFO, "Coords of point [ %.3f, %.3f ]", point.x, point.y);
				
				// Get barycentric coordinates
				DgBary3 bary = DgVec2Bary3(p1, p2, p3, point);
				
				//DgLog(DG_LOG_INFO, "Bary ( %.3f : %.3f : %.3f ) = %.3f", bary.u, bary.v, bary.w, bary.u + bary.v + bary.w);
				
				//DgLog(DG_LOG_INFO, "Bary after: [ %.3f, %.3f ]", bary.u * p1.x + bary.v * p2.x + bary.w * p3.x, bary.u * p1.y + bary.v * p2.y + bary.w * p3.y);
				
				// Check for collision
				if ((bary.u >= 0.0f) && (bary.v >= 0.0f) && (bary.w >= 0.0f)) {
					// Evaluate pixel colour based on barycentric coordinates
					DgColour c = DgVec4Bary3Evaluate(bary.u, &tri->p1.colour, bary.v, &tri->p2.colour, bary.w, &tri->p3.colour);
					
					// Draw the pixel
					DgBitmapDrawPixel(this, x, y, c);
				}
			}
		}
	}
}

void DgBitmapFill(DgBitmap * restrict this, DgVec4 colour) {
	/**
	 * Fill a bitmap with a given colour.
	 * 
	 * @param this Bitmap to draw to
	 * @param colour The colour to fill the image with
	 */
	
	for (size_t x = 0; x < this->width; x++) {
		for (size_t y = 0; y < this->height; y++) {
			DgBitmapDrawPixel(this, x, y, colour);
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
