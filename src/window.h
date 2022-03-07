/**
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * Windowing access
 */

#pragma once

#include <inttypes.h>
#include <stddef.h>

#include <SDL2/SDL.h>

#include "maths.h"

/**
 * Window
 * ======
 * 
 * A window contains information needed to maintain a window.
 */
typedef struct DgWindow {
	SDL_Window *window;
	SDL_Surface *surface;
	DgVec2I size;
} DgWindow;

uint32_t DgWindowInit(DgWindow *this, const char *title, DgVec2I size);
void DgWindowFree(DgWindow *this);
int32_t DgWindowUpdate(DgWindow *this, DgBitmap *bitmap);
