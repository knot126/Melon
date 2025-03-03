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

#include "texture.h"

typedef struct { uint8_t r, g, b; } RGBPixel;
typedef struct { uint8_t r, g, b, a; } RGBAPixel;

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
		if (c == 0xff) {
			prev.r = DgStreamReadUInt8(stream, NULL);
			prev.g = DgStreamReadUInt8(stream, NULL);
			prev.b = DgStreamReadUInt8(stream, NULL);
			prev.a = DgStreamReadUInt8(stream, NULL);
			QOI_TABLE_INSERT(prev);
			QOI_PUT(prev);
		}
		
		// Index
		if ((c >> 6) == 0b00) {
			prev = recent[(c & 0b111111)];
			QOI_PUT(prev);
		}
		
		// Diff
		if ((c >> 6) == 0b01) {
			prev.r += ((c >> 4) & 0b11) - 2;
			prev.g += ((c >> 2) & 0b11) - 2;
			prev.b += ((c) & 0b11) - 2;
			QOI_TABLE_INSERT(prev);
			QOI_PUT(prev);
		}
		
		// Luma
		if ((c >> 6) == 0b10) {
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
		if ((c >> 6) == 0b11) {
			uint8_t run_len = (c & 0b111111) + 1;
			
			for (size_t j = 0; j < run_len && i < this->width * this->height; i++, j++) {
				QOI_PUT(prev);
			}
		}
	}
	
	return true;
}
