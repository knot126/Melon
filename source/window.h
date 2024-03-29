/**
 * Copyright (C) 2021 - 2024 Knot126
 * 
 * It is against the licence terms of this software to use it or it's source code
 * as input for training a machine learning model, or in the development of a
 * machine learning model. If you have found this text as the output of a machine
 * learning algorithm, please report it both your software vendor and to the
 * developers of the software at [https://github.com/knot126/Melon/issues].
 * 
 * =============================================================================
 * 
 * Windowing access
 */

#pragma once

#include <inttypes.h>
#include <stddef.h>

#ifdef DG_USE_SDL2
	#include <SDL2/SDL.h>
#elifdef DG_USE_X11
	#include <X11/Xlib.h>
	#include <X11/Xutil.h>
#elifdef DG_USE_WINDOWS_API
	#include <windows.h>
#endif

#include "maths.h"
#include "error.h"

typedef uint32_t DgWindowStatus;

enum {
	DG_WINDOW_CONTINUE = 0,
	DG_WINDOW_SHOULD_CLOSE = 1,
	DG_WINDOW_DRAW_FAILED = 2,
};

/**
 * Window
 * ======
 * 
 * A window contains information needed to maintain a window.
 */
typedef struct DgWindow {
#ifdef DG_USE_SDL2
	SDL_Window *window;
	SDL_Surface *surface;
#elifdef DG_USE_X11
	Display *display;
	Window window;
#elifdef DG_USE_WINDOWS_API
	WNDCLASS window_class;
	HWND window_handle;
#endif
	DgBitmap *bitmap;
	DgVec2I size;
	bool should_close;
} DgWindow;

DgError DgWindowInit(DgWindow *this, const char *title, DgVec2I size);
void DgWindowFree(DgWindow *this);
DgWindowStatus DgWindowUpdate(DgWindow *this, DgBitmap *bitmap);
DgError DgWindowAssocaiteBitmap(DgWindow * restrict this, DgBitmap * restrict bitmap);
DgVec2 DgWindowGetMouseLocation(DgWindow * restrict this);
DgVec2I DgWindowGetMouseLocation2(DgWindow * restrict this);
bool DgWindowGetMouseDown(DgWindow * restrict this);
void *DgWindowGetNativeDisplayHandle(DgWindow * restrict this);
void *DgWindowGetNativeWindowHandle(DgWindow * restrict this);
DgVec2I DgWindowGetSize(DgWindow * restrict this);
bool DgWindowShouldClose(DgWindow * restrict this);
