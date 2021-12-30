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

bool DgBitmapNew(DgBitmap *bitmap, const uint16_t width, const uint16_t height, const uint16_t chan) {
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
// 	DgLog(DG_LOG_VERBOSE, "DgBitmap debug: Allocate 0x%X bytes of memory for %dx%dx%d bitmap at <0x%X>.", bitmap->alloc_sz, width, height, chan, bitmap->src);
	
	if (!bitmap->src) {
		DgLog(DG_LOG_ERROR, "Failed to allocate 0x%X bytes of memory for bitmap.", bitmap->alloc_sz);
		return true;
	}
	
	uint8_t *src = bitmap->src;
	
	return false;
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
	
	// Invert for graph-like coordinates
	y = (this->height - y) - 1;
	size_t pixel_offset = ((y * this->width) + x) * this->chan;
	
	this->src[pixel_offset + 0] = (uint8_t) (colour.r * 255.0f);
	if (this->chan >= 2) {
		this->src[pixel_offset + 1] = (uint8_t) (colour.g * 255.0f);
		if (this->chan >= 3) {
			this->src[pixel_offset + 2] = (uint8_t) (colour.b * 255.0f);
			if (this->chan >= 4) {
				this->src[pixel_offset + 3] = (uint8_t) (colour.a * 255.0f);
			}
		}
	}
	
// 	DgLog(DG_LOG_VERBOSE, "draw at %d, %d", x, y);
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
	y = this->width - y;
	size_t pixel_offset = ((y * this->width) + x) * this->chan;
	
	colour->r = (((float)this->src[pixel_offset + 0]) * 255.0f);
	if (this->chan >= 2) {
		colour->g = (((float)this->src[pixel_offset + 1]) * 255.0f);
		if (this->chan >= 3) {
			colour->b = (((float)this->src[pixel_offset + 2]) * 255.0f);
			if (this->chan >= 4) {
				colour->a = (((float)this->src[pixel_offset + 3]) * 255.0f);
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

void DgBitmapFill(DgBitmap *this, DgVec4 colour) {
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

void DgBitmapAntiAliasX(DgBitmap *this) {
	/**
	 * Preform basic anti-aliasing on the bitmap.
	 * 
	 * @param this Bitmap to antialias
	 */
	
	DgVec4 current, l, u, r, d;
	
	for (size_t y = 0; y < this->height; y++) {
		for (size_t x = 0; x < this->height; x++) {
			DgBitmapGetPixel(this, x, y, &current);
			DgBitmapGetPixel(this, x - 1, y, &l);
			DgBitmapGetPixel(this, x, y + 1, &u);
			DgBitmapGetPixel(this, x + 1, y, &r);
			DgBitmapGetPixel(this, x, y - 1, &d);
			
			current = DgVec4Add(DgVec4Scale(0.125f, d), DgVec4Add(DgVec4Scale(0.125f, r), DgVec4Add(DgVec4Scale(0.125f, u), DgVec4Add(DgVec4Scale(0.125f, u), DgVec4Add(DgVec4Scale(0.125f, l), DgVec4Scale(0.5f, current))))));
			
			DgBitmapDrawPixel(this, x, y, current);
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

// =============================================================================
// =============================================================================
// =============================================================================

DgBitmap *DgBitmapGenTiles(const uint16_t width, const uint16_t height, const uint16_t size) {
	/**
	 * Allocates and generates a bitmap in a tile format. The image will have
	 * three channels (RGB) and be 8-bit.
	 * 
	 * @param width The width of the generated bitmap
	 * @param height The height of the generated bitmap
	 * @param size The size of the tiles on the bitmap
	 * @return Dyamically allocated bitmap
	 */
	
	DgBitmap *bitmap = (DgBitmap *) DgAlloc(sizeof(DgBitmap));
	
	if (!bitmap) {
		return NULL;
	}
	
	bitmap->width = width;
	bitmap->height = height;
	bitmap->chan = DG_BITMAP_DEFAULT_CHANNELS_COUNT;
	bitmap->src = (uint8_t *) DgAlloc(width * height * DG_BITMAP_DEFAULT_CHANNELS_COUNT * sizeof(uint8_t));
	
	if (!bitmap->src) {
		DgFree(bitmap);
		return NULL;
	}
	
	uint8_t *src = bitmap->src;
	
	uint16_t size_tick_x = size;
	uint16_t size_tick_y = size;
	uint8_t current_x = 0;
	uint8_t current_y = 0;
	
	for (uint16_t x = 0; x < width; x++, size_tick_x--) {
		for (uint16_t y = 0; y < height; y++, size_tick_y--) {
			src[(x * width * 3) + (y * 3) + 0] = current_y;
			src[(x * width * 3) + (y * 3) + 1] = current_y;
			src[(x * width * 3) + (y * 3) + 2] = current_y;
			
			if (size_tick_y <= 0) {
				current_y = ~current_y;
				size_tick_y = size;
			}
		}
		
		if (size_tick_x <= 0) {
			current_x = ~current_x;
			size_tick_x = size;
		}
		
		current_y = current_x;
	}
	
	return bitmap;
}

DgBitmap *DgBitmapRandom(const uint16_t width, const uint16_t height) {
	/**
	 * Generate a random bitmap.
	 * 
	 * @param width The width of the bitmap
	 * @param height The height of the bitmap
	 * @return Dynamically allocated bitmap
	 */
	
	DgBitmap *bitmap = (DgBitmap *) DgAlloc(sizeof(DgBitmap));
	
	if (!bitmap) {
		return NULL;
	}
	
	bitmap->width = width;
	bitmap->height = height;
	bitmap->chan = DG_BITMAP_DEFAULT_CHANNELS_COUNT;
	bitmap->src = (uint8_t *) DgAlloc(width * height * DG_BITMAP_DEFAULT_CHANNELS_COUNT * sizeof(uint8_t));
	
	if (!bitmap->src) {
		DgFree(bitmap);
		return NULL;
	}
	
	uint8_t *src = bitmap->src;
	
	for (uint32_t i = 0; i < (bitmap->width * bitmap->height * bitmap->chan); i++) {
		src[i] = (uint8_t) DgRandInt() % 256;
	}
	
	return bitmap;
}
