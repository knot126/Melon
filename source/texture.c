/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Images and textures
 */

#include "common.h"
#include "memory.h"
#include "string.h"
#include "stream.h"
#include "file.h"

#include "texture.h"

typedef DgRGBPixel RGBPixel;
typedef DgRGBAPixel RGBAPixel;

void DgTextureFree(DgTexture *this) {
	DgMemoryFree(this->pixels);
	DgMemoryZero(this, sizeof *this);
}

#define QOI_TABLE_INSERT(P) recent[(3 * P.r + 5 * P.g + 7 * P.b + 11 * P.a) & 0b111111] = P;
#define QOI_PUT(P) { if (this->format == 4) { ((RGBAPixel *) this->pixels)[i] = P; } else { ((RGBPixel *) this->pixels)[i] = (RGBPixel) {P.r, P.g, P.b}; } }

bool DgTextureLoadQOIFromStream(DgTexture *this, DgStream *stream) {
	/**
	 * Load a Quite OK Image Format texture from a stream
	 */
	
	DgMemoryZero(this, sizeof *this);
	
	// Format is BE
	DgStreamSetEndian(stream, DG_ENDIAN_BIG);
	
	char magic[5] = { 0 };
	DgStreamRead(stream, 4, magic);
	
	if (!DgStringEqual(magic, "qoif")) {
		return false;
	}
	
	// Read width and height
	this->width = DgStreamReadUInt32(stream, NULL);
	this->height = DgStreamReadUInt32(stream, NULL);
	
	// Read channels and colourspace info
	this->format = DgStreamReadUInt8(stream, NULL);
	DgStreamReadUInt8(stream, NULL);
	
	// Try allocating memory
	this->pixels = DgMemoryAllocate(this->width * this->height * this->format);
	
	if (!this->pixels) {
		return false;
	}
	
	// Decode
	RGBAPixel prev = (RGBAPixel) {0, 0, 0, 255};
	RGBAPixel recent[64] = { 0 };
	
	for (size_t i = 0; i < this->width * this->height; i++) {
		uint8_t c = DgStreamReadUInt8(stream, NULL);
		
		// RGB pair
		if (c == 0xfe) {
			prev.r = DgStreamReadUInt8(stream, NULL);
			prev.g = DgStreamReadUInt8(stream, NULL);
			prev.b = DgStreamReadUInt8(stream, NULL);
			QOI_TABLE_INSERT(prev);
			QOI_PUT(prev);
		}
		
		// RGBA pair
		else if (c == 0xff) {
			prev.r = DgStreamReadUInt8(stream, NULL);
			prev.g = DgStreamReadUInt8(stream, NULL);
			prev.b = DgStreamReadUInt8(stream, NULL);
			prev.a = DgStreamReadUInt8(stream, NULL);
			QOI_TABLE_INSERT(prev);
			QOI_PUT(prev);
		}
		
		// Index
		else if ((c >> 6) == 0b00) {
			prev = recent[(c & 0b111111)];
			QOI_PUT(prev);
		}
		
		// Diff
		else if ((c >> 6) == 0b01) {
			prev.r += ((c >> 4) & 0b11) - 2;
			prev.g += ((c >> 2) & 0b11) - 2;
			prev.b += ((c) & 0b11) - 2;
			QOI_TABLE_INSERT(prev);
			QOI_PUT(prev);
		}
		
		// Luma
		else if ((c >> 6) == 0b10) {
			// 10 ------ ---- ----
			// ^      ^  ^    ^
			// Header |  Red  Blue diff (as offset from negative green diff)
			//        Green diff
			uint8_t diffg = (c & 0b111111) - 32;
			c = DgStreamReadUInt8(stream, NULL);
			uint8_t diffr = ((c >> 4) - 8) + diffg;
			uint8_t diffb = ((c & 0b1111) - 8) + diffg;
			prev.r += diffr;
			prev.g += diffg;
			prev.b += diffb;
			QOI_TABLE_INSERT(prev);
			QOI_PUT(prev);
		}
		
		// Run
		else if ((c >> 6) == 0b11) {
			uint8_t run_len = (c & 0b111111) + 1;
			
			for (size_t j = 0; (j < run_len) && (i < this->width * this->height); i++, j++) {
				QOI_PUT(prev);
			}
			
			i--;
		}
	}
	
	return true;
}

bool DgTextureLoadQOI(DgTexture *this, const char *path) {
	/**
	 * Load a QOI file into a texture
	 */
	
	DgStream stream;
	DgError error = DgFileOpen(&stream, path, DG_STREAM_READ);
	
	if (error) {
		return false;
	}
	
	bool success = DgTextureLoadQOIFromStream(this, &stream);
	
	DgStreamClose(&stream);
	
	return success;
}

#define DG_QOG_REMAINING (this->width * this->height - i)

bool DgTextureWriteQOGToStream(DgTexture *this, DgStream *stream) {
	/**
	 * Write a prototype greyscale QOI-based format to the given stream
	 */
	
	if (this->format != DG_TEXTURE_L) {
		return false;
	}
	
	DgStreamWriteString(stream, "qogf");
	DgStreamWriteUInt16(stream, this->width);
	DgStreamWriteUInt16(stream, this->height);
	
	uint8_t prev = 0;
	
	for (size_t i = 0; i < this->width * this->height; i++) {
		// Consider RLE
		size_t runlen = 0;
		
		for (runlen = 0; i + runlen < this->width * this->height; runlen++) {
			if (this->pixels[i + runlen] != prev) {
				break;
			}
		}
		
		if (runlen) {
			if (runlen > 319) {
				runlen = 319;
			}
			
			if (runlen < 63) {
				DgStreamWriteUInt8(stream, 0xc0 | (runlen - 1));
			}
			else {
				DgStreamWriteUInt8(stream, 0xfe);
				DgStreamWriteUInt8(stream, runlen - 63);
			}
			
			continue;
		}
		
		// Consider tri-pixel delta encoding
		if (DG_QOG_REMAINING >= 3) {
			if ((this->pixels[i] - prev + 2) < 4 &&
				(this->pixels[i+1] - this->pixels[i] + 2) < 4 &&
				(this->pixels[i+2] - this->pixels[i+1] + 2) < 4) {
				DgStreamWriteUInt8(stream, ((this->pixels[i] - prev + 2) << 4) | ((this->pixels[i+1] - this->pixels[i] + 2) << 2) | ((this->pixels[i+2] - this->pixels[i+1] + 2)));
				prev = this->pixels[i+2];
				i += 2;
				continue;
			}
		}
		
		// Consider dual-pixel delta encoding
		if (DG_QOG_REMAINING >= 2) {
			if ((this->pixels[i] - prev + 4) < 8 && (this->pixels[i+1] - this->pixels[i] + 4) < 8) {
				DgStreamWriteUInt8(stream, 0x40 | ((this->pixels[i] - prev + 4) << 3) | ((this->pixels[i+1] - this->pixels[i] + 4)));
				prev = this->pixels[i+1];
				i++;
				continue;
			}
		}
		
		// Try single pixel delta
		if ((this->pixels[i] - prev + 32) < 64) {
			DgStreamWriteUInt8(stream, 0x80 | (this->pixels[i] - prev + 32));
			prev = this->pixels[i];
			continue;
		}
		
		// aw damn it
		DgStreamWriteUInt8(stream, 0xff);
		DgStreamWriteUInt8(stream, this->pixels[i]);
		
		prev = this->pixels[i];
	}
	
	return true;
}

#undef DG_QOG_REMAINING

bool DgTextureLoadTGAFromStream(DgTexture *this, DgStream *stream) {
	DgMemoryZero(this, sizeof *this);
	
	// TGA is little endian (or so I think, anyway)
	DgStreamSetEndian(stream, DG_ENDIAN_LITTLE);
	
	uint8_t info_size = DgStreamReadUInt8(stream, NULL);
	uint8_t colmap_type = DgStreamReadUInt8(stream, NULL);
	uint8_t image_type = DgStreamReadUInt8(stream, NULL);
	
	// Colourmaps are not supported
	if (colmap_type != 0) {
		return false;
	}
	
	// Enforce image type to be uncompressed truecolour
	if (image_type != 2) {
		return false;
	}
	
	// Skip colormap info
	DgStreamSkip(stream, 5);
	
	// Hotspot (ignored)
	uint16_t hot_x = DgStreamReadUInt16(stream, NULL);
	uint16_t hot_y = DgStreamReadUInt16(stream, NULL);
	
	// Size info
	uint16_t width = DgStreamReadUInt16(stream, NULL);
	uint16_t height = DgStreamReadUInt16(stream, NULL);
	uint8_t depth = DgStreamReadUInt8(stream, NULL); // in bits per pixel
	uint8_t extra = DgStreamReadUInt8(stream, NULL);
	
	// Skip image ID field
	DgStreamSkip(stream, info_size);
	
	// Derive format info
	uint8_t alpha_depth = (extra & 0xf);
	bool from_right = (extra >> 4);
	bool from_top = (extra >> 5);
	
	// We only support RGB and RGBA
	if (depth != 24 && depth != 32) {
		return false;
	}
	
	// Have sanity
	if (from_right) {
		return false;
	}
	
	// Store format (RGB or RGBA) and stuff
	this->format = (depth >> 3);
	this->width = width;
	this->height = height;
	
	// Allocate memory for the bitmap
	this->pixels = DgMemoryAllocate(this->width * this->height * this->format);
	
	if (!this->pixels) {
		return false;
	}
	
	// If we read from the top, we just need to copy one for one since it's
	// otherwise the same format.
	if (from_top) {
		DgStreamRead(stream, this->width * this->height * this->format, this->pixels);
	}
	// Otherwise we read one scanline at a time to flip things around.
	else {
		for (size_t i = 0; i < this->height; i++) {
			DgStreamRead(stream, this->width * this->format, this->pixels + ((this->height - i - 1) * this->width * this->format));
		}
	}
	
	return true;
}

bool DgTextureGenerateTiles(DgTexture *this) {
	this->format = DG_TEXTURE_RGB;
	this->width = 256;
	this->height = 256;
	this->pixels = DgMemoryAllocate(this->width * this->height * this->format);
	
	if (!this->pixels) {
		return false;
	}
	
	for (size_t y = 0; y < this->height; y++) {
		for (size_t x = 0; x < this->width; x++) {
			uint8_t v = (((x / (this->width / 8)) & 1) ^ ((y / (this->height / 8)) & 1)) ? 0xff : 0;
			((RGBPixel *) this->pixels)[this->width * y + x] = (RGBPixel) {v, v, v};
		}
	}
	
	return true;
}

#define DG_MAX(A, B) ((A) > (B) ? (A) : (B))

bool DgTextureGreyscale(DgTexture *this) {
	/**
	 * Convert a texture to greyscale
	 */
	
	if (this->format == DG_TEXTURE_L) {
		return true;
	}
	
	uint8_t *new_pixels = DgMemoryAllocate(this->width * this->height);
	
	if (!new_pixels) {
		return false;
	}
	
	for (size_t i = 0; i < this->width * this->height; i++) {
		if (this->format > 3) {
			new_pixels[i] = DG_MAX(DG_MAX(this->pixels[this->format * i], this->pixels[this->format * i + 1]), this->pixels[this->format * i + 2]);
		}
		else {
			new_pixels[i] = this->pixels[this->format * i];
		}
	}
	
	DgMemoryFree(this->pixels);
	this->pixels = new_pixels;
	this->format = DG_TEXTURE_L;
	
	return true;
}

#undef DG_MAX
