/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Windowing access
 */

#pragma once

#include <inttypes.h>
#include <stddef.h>

// #if defined(DG_ENABLE_X11)
// 	#include <X11/Xlib.h>
// 	#include <X11/Xutil.h>
// #endif

#if defined(DG_ENABLE_WAYLAND)
	#include <wayland-client-core.h>
#endif

#ifdef _WIN32
	#include <windows.h>
#endif

#include "maths.h"
#include "error.h"
#include "library.h"

typedef enum : uint32_t {
	DG_WINDOW_NO_BACKEND = 0,
	DG_WINDOW_X11 = 1,
	DG_WINDOW_WAYLAND = 2,
	DG_WINDOW_WINDOWS = 3,
} DgWindowBackend;

typedef enum : uint32_t {
	DG_MOUSE_LEFT_DOWN = (1 << 0),
	DG_MOUSE_MIDDLE_DOWN = (1 << 1),
	DG_MOUSE_RIGHT_DOWN = (1 << 2),
} DgWindowMouseStateFlags;

/**
 * Window
 * ======
 * 
 * A window contains information needed to maintain a window.
 */
typedef struct DgWindow {
	DgWindowBackend backend;
#ifdef __linux__
	union {
#if defined(DG_ENABLE_X11)
		struct {
			DgLibrary lib;
			void *display;
			uint32_t window; // X headers say this should be 32-bit unsigned, so
			                 // just make it and don't bother with X11 headers.
		} x11;
#endif
#if defined(DG_ENABLE_WAYLAND)
		struct {
			struct wl_display *display;
			struct wl_surface *surface;
		} wl;
#endif
	};
#elif defined(_WIN32)
	WNDCLASS window_class;
	HWND window_handle;
#endif
	DgVec2I size;
	DgVec2I mouse_pos;
	bool should_close;
} DgWindow;

DgError DgWindowInit(DgWindow *this, const char *title, DgVec2I size);
void DgWindowFree(DgWindow *this);
bool DgWindowUpdate(DgWindow *this);

DgVec2I DgWindowGetMouseLocation(DgWindow * restrict this);
bool DgWindowGetMouseDown(DgWindow * restrict this);

DgVec2I DgWindowGetSize(DgWindow * restrict this);
bool DgWindowShouldClose(DgWindow * restrict this);

DgWindowBackend DgWindowGetBackend(DgWindow *this);
void *DgWindowGetNativeDisplayHandleForEGL(DgWindow * restrict this);
void *DgWindowGetNativeWindowHandleForEGL(DgWindow * restrict this);
