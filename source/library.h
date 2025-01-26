/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Dynamic library loading
 */

#pragma once

#define DG_LIBRARY_OWN_MODULE NULL

typedef struct DgLibrary {
	void *handle;
} DgLibrary;

DgError DgLibraryInit(DgLibrary * const restrict this, const char * const restrict path);
DgError DgLibraryFree(DgLibrary * const restrict this);
void *DgLibraryGetSymbol(DgLibrary * const restrict this, const char * const restrict symbol);

void *DgGetSymbol(const char * const symbol);
