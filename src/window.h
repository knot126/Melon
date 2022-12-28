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
#elif defined(DG_USE_WINDOWS_API)
	#include <windows.h>
#endif

#include "maths.h"
#include "error.h"

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
#elif defined(DG_USE_WINDOWS_API)
	WNDCLASS window_class;
	HWND window_handle;
#endif
	DgVec2I size;
} DgWindow;

uint32_t DgWindowInit(DgWindow *this, const char *title, DgVec2I size);
void DgWindowFree(DgWindow *this);
int32_t DgWindowUpdate(DgWindow *this, DgBitmap *bitmap);
DgError DgWindowAssocaiteBitmap(DgWindow * restrict this, DgBitmap * restrict bitmap);
DgVec2 DgWindowGetMouseLocation(DgWindow * restrict this);
DgVec2I DgWindowGetMouseLocation2(DgWindow * restrict this);
bool DgWindowGetMouseDown(DgWindow * restrict this);
