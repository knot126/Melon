/**
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * Windowing access
 */

#pragma once

#include <inttypes.h>
#include <stddef.h>

#if !defined(DG_NO_SDL)
	#include <SDL2/SDL.h>
#endif

#include "maths.h"

/**
 * Window
 * ======
 * 
 * A window contains information needed to maintain a window.
 */
typedef struct DgWindow {
#if !defined(DG_NO_SDL)
	SDL_Window *window;
	SDL_Surface *surface;
#endif
	DgVec2I size;
} DgWindow;

uint32_t DgWindowInit(DgWindow *this, const char *title, DgVec2I size);
void DgWindowFree(DgWindow *this);
int32_t DgWindowUpdate(DgWindow *this, DgBitmap *bitmap);
