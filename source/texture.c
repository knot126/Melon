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

void DgTextureFree(DgTexture *this) {
	DgMemoryFree(this->pixels);
	DgMemoryZero(this, sizeof *this);
}
