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
 * 
 * @warning Not good! I *just* wanted it to work.
 * 
 * @see https://benedicthenshaw.com/soft_render_sdl2.html
 */

#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>

#include "log.h"
#include "maths.h"
#include "bitmap.h"
#include "error.h"

#include "window.h"

#if defined(DG_USE_SDL2) || defined(DG_USE_X11)

#ifdef DG_USE_SDL2
#include <SDL2/SDL.h>

static uint32_t gWindowCount_ = 0;
#elifdef DG_USE_X11
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <signal.h>
#endif

DgError DgWindowInit(DgWindow *this, const char *title, DgVec2I size) {
	/**
	 * Initialise and create a window
	 * 
	 * @param this Window object
	 * @return Zero on success, non-zero on failure
	 */
	
#ifdef DG_USE_SDL2
	gWindowCount_++;
	
	if (!SDL_WasInit(0)) {
		SDL_Init(SDL_INIT_VIDEO);
	}
	
	this->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.x, size.y, 0);
	this->surface = SDL_GetWindowSurface(this->window);
#elifdef DG_USE_X11
	this->display = XOpenDisplay(NULL);
	
	if (!this->display) {
		return DG_ERROR_FAILED;
	}
	
	int screen = DefaultScreen(this->display);
	Window root = RootWindow(this->display, screen);
	Visual *visual = DefaultVisual(this->display, screen);
	Colormap colourmap = XCreateColormap(this->display, root, visual, AllocNone);
	
	XSetWindowAttributes attributes;
	attributes.colormap = colourmap;
	attributes.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask;
	
	this->window = XCreateWindow(this->display, root, 0, 0, size.x, size.y, 0, DefaultDepth(this->display, screen), InputOutput, visual, CWColormap | CWEventMask, &attributes);
	
	XFreeColormap(this->display, colourmap);
	
	XMapWindow(this->display, this->window);
	XStoreName(this->display, this->window, title);
	
	if (!this->window) {
		return DG_ERROR_FAILED;
	}
#endif
	
	this->size = size;
	this->should_close = false;
	
	return DG_ERROR_SUCCESSFUL;
}

void DgWindowFree(DgWindow *this) {
	/**
	 * Free resources assocaited with a window
	 * 
	 * @param this Window object
	 */
	
#ifdef DG_USE_SDL2
	gWindowCount_--;
	
	if (gWindowCount_ == 0) {
		SDL_Quit();
	}
#elifdef DG_USE_X11
	XDestroyWindow(this->display, this->window);
	XCloseDisplay(this->display);
#endif
}

DgWindowStatus DgWindowUpdate(DgWindow *this, DgBitmap *bitmap) {
	/**
	 * Display new changes to a window.
	 * 
	 * @note bitmap can be NULL if you have an assocaited bitmap (or you just
	 * like to draw unknown memory...)
	 * 
	 * @warning The currently active bitmap must not have less pixels than the
	 * window surface or there will be a buffer overrun.
	 * 
	 * @warning The currently active bitmap must be 24-bits.
	 * 
	 * @param this Window object
	 * @return DG_WINDOW_CONTINUE if this window draw succeeded
	 *         DG_WINDOW_DRAW_FAILED if drawing to the window failed
	 *         DG_WINDOW_SHOULD_CLOSE if the window is expected to close
	 */
	
#ifdef DG_USE_SDL2
	// Check up on events
	SDL_Event event;
	
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT: {
				this->should_close = true;
				return DG_WINDOW_SHOULD_CLOSE;
				break;
			}
		}
	}
	
	// Draw the window
	if (bitmap) {
		uint32_t *wd = (uint32_t *) this->surface->pixels;
		uint32_t pixel_count = this->size.x * this->size.y;
		uint8_t *data = bitmap->src;
		
		for (size_t i = 0; i < pixel_count; i++) {
			wd[i] = SDL_MapRGBA(this->surface->format, data[(i * 3)], data[(i * 3) + 1], data[(i * 3) + 2], 0xff);
		}
	}
	
	return SDL_UpdateWindowSurface(this->window) ? DG_WINDOW_DRAW_FAILED : DG_WINDOW_CONTINUE;
#elifdef DG_USE_X11
	while (XPending(this->display)) {
		XEvent event;
		
		XNextEvent(this->display, &event);
		
		if (event.type == KeyPress) {
			this->should_close = true;
		}
		else if (event.type == DestroyNotify) {
			this->should_close = true;
		}
	}
	
	return DG_WINDOW_CONTINUE;
#endif
}

DgError DgWindowAssocaiteBitmap(DgWindow * restrict this, DgBitmap * restrict bitmap) {
	/**
	 * Assocaite a bitmap object with a window for faster rendering.
	 * 
	 * @note To use the bitmap, you must call DgWindowUpdate with a bitmap of
	 * NULL otherwise it just copies normally.
	 * 
	 * @param this Window object
	 * @param bitmap Bitmap object
	 */
	
#ifdef DG_USE_SDL2
	DgBitmapSetSource(bitmap, this->surface->pixels, this->size, 4);
	
	return DG_ERROR_SUCCESSFUL;
#else
	return DG_ERROR_NOT_IMPLEMENTED;
#endif
}

DgVec2 DgWindowGetMouseLocation(DgWindow * restrict this) {
	/**
	 * Return the current mouse position.
	 * 
	 * @param this Window object
	 * @return Cursor position relative to window
	 */
	
#ifdef DG_USE_SDL2
	int x, y;
	
	SDL_PumpEvents();
	SDL_GetMouseState(&x, &y);
	
	return (DgVec2) {(float)(x) / (float)(this->size.x), ((float)(y) / (float)(this->size.y))};
#else
	return (DgVec2) {0.0, 0.0};
#endif
}

DgVec2I DgWindowGetMouseLocation2(DgWindow * restrict this) {
	/**
	 * Return the current mouse position in window coordinates.
	 * 
	 * @param this Window object (not needed using SDL backend)
	 * @return Cursor position relative to window in window coordinates
	 */
	
#ifdef DG_USE_SDL2
	int x, y;
	
	SDL_PumpEvents();
	SDL_GetMouseState(&x, &y);
	
	return (DgVec2I) {x, y};
#else
	return (DgVec2I) {0, 0};
#endif
}

bool DgWindowGetMouseDown(DgWindow * restrict this) {
	/**
	 * Get if the left mouse button is down or not.
	 * 
	 * @param this Window object (not needed using SDL backend)
	 * @return Cursor position relative to window
	 */
	
#if DG_USE_SDL2
	SDL_PumpEvents();
	return !!(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK);
#else
	return false;
#endif
}

void *DgWindowGetNativeDisplayHandle(DgWindow * restrict this) {
	/**
	 * Get the native display handle used with EGL.
	 * 
	 * @param this Window object
	 * @return Handle to the native display object 
	 */
	
#ifdef DG_USE_X11
	return (void *) this->display;
#else
	return NULL;
#endif
}

void *DgWindowGetNativeWindowHandle(DgWindow * restrict this) {
	/**
	 * Get the native window handle used with EGL.
	 * 
	 * @param this Window object
	 * @return Handle to the native window object 
	 */
	
	return (void *) this->window;
}

DgVec2I DgWindowGetSize(DgWindow * restrict this) {
	/**
	 * Get the current size of the window.
	 * 
	 * @param this Window object
	 * @return Window size in pixels
	 */
	
	return this->size;
}

bool DgWindowShouldClose(DgWindow * restrict this) {
	/**
	 * Return if the window should close
	 * 
	 * @param this Window object
	 * @return true if the window should close, false if the window should keep
	 * running
	 */
	
	return this->should_close;
}

#elif defined(DG_USE_WINDOWS_API)

#include <windows.h>

const char DG_WINDOW_CLASS_NAME[] = "Melon Library Window";
bool DG_WINDOW_WANTS_TO_QUIT = false;

static LRESULT CALLBACK DgWindow_NTProcessWindowEvent(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam) {
	if (message == WM_CLOSE || message == WM_DESTROY || message == WM_QUIT || message == WM_NCDESTROY) {
		DG_WINDOW_WANTS_TO_QUIT = true;
	}
	
	return DefWindowProc(window_handle, message, wparam, lparam);
}

DgError DgWindowInit(DgWindow *this, const char *title, DgVec2I size) {
	/**
	 * Create a window (Windows NT)
	 * 
	 * @param this Handle to the window object
	 * @param title Title of the window
	 * @param size Size of the window
	 */
	
	// Setup window class
	memset(&this->window_class, 0, sizeof this->window_class);
	this->window_class.lpfnWndProc = &DgWindow_NTProcessWindowEvent; // Default callback
	this->window_class.hInstance = GetModuleHandle(NULL); // It seems this can be NULL and it will take care of things.
	this->window_class.lpszClassName = DG_WINDOW_CLASS_NAME;
	
	// Register the class
	if (RegisterClass(&this->window_class) == 0) {
		DgLog(DG_LOG_VERBOSE, "RegisterClass: error code = %d", GetLastError());
		return DG_ERROR_FAILED;
	}
	
	this->window_handle = CreateWindowEx(
		0,
		DG_WINDOW_CLASS_NAME,
		title,
		WS_POPUP, // HACK when using WS_OVERLAPPEDWINDOW windows needs extra size added to the window
		CW_USEDEFAULT, CW_USEDEFAULT,
		size.x, size.y,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL
	);
	
	if (this->window_handle == NULL) {
		DgLog(DG_LOG_VERBOSE, "CreateWindowEx: error code = %d", GetLastError());
		return DG_ERROR_FAILED;
	}
	
	// Pop up the window
	ShowWindow(this->window_handle, SW_SHOW);
	
	return DG_ERROR_SUCCESS;
}

void DgWindowFree(DgWindow *this) {
	return;
}

DgWindowStatus DgWindowUpdate(DgWindow *this, DgBitmap *bitmap) {
	/**
	 * Update the window contents with to use the given bitmap (or NULL if using
	 * the assocaited bitmap).
	 * 
	 * @param this Window object
	 * @param bitmap Bitmap to take image from
	 * @return Error code
	 */
	
	// Handle messages
	MSG message;
	
	// Invalidate the rectange so we can draw to it.
	InvalidateRect(this->window_handle, NULL, FALSE);
	
	// Find the correct bitmap
	bitmap = bitmap ? bitmap : this->bitmap;
	
	// If there is no bitmap, be an asre about it
	if (!bitmap) {
		DgLog(DG_LOG_FATAL, "No bitmap for DgWindowUpdate!!");
		return 1;
	}
	
	while (PeekMessage(&message, this->window_handle, 0, 0, PM_NOREMOVE) == 1) {
		// Any extra handles for the message
		if (message.message == WM_PAINT) {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(this->window_handle, &ps);
			
			int width = bitmap->width;
			int height = bitmap->height;
			
			BITMAPINFOHEADER psHeaderGlobal = {0};
			psHeaderGlobal.biSize = sizeof(BITMAPINFOHEADER);
			psHeaderGlobal.biWidth = width;
			psHeaderGlobal.biHeight = height;
			psHeaderGlobal.biPlanes = 1;
			psHeaderGlobal.biBitCount = 24;
			
			BITMAPINFOHEADER* psHeader = &psHeaderGlobal;
			
			SetDIBitsToDevice(hdc, 0, 0, width, height, 0, 0, 0, height, (void *) bitmap->src, (BITMAPINFO *) psHeader, DIB_RGB_COLORS);
			
			EndPaint(this->window_handle, &ps);
			break;
		}
		
		// Remove message
		PeekMessage(&message, this->window_handle, 0, 0, PM_REMOVE);
		
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	
	// HACK: We need to use a global variable to check if the window would like
	// to quit becuase Windows does not use PostMessage to push the WM_QUIT or
	// similar messages and instead calls the callback directly, which is not
	// compatible with our architecture.
	if (DG_WINDOW_WANTS_TO_QUIT) {
		return 1;
	}
	
	return 0;
}

DgError DgWindowAssocaiteBitmap(DgWindow * restrict this, DgBitmap * restrict bitmap) {
	/**
	 * Assocaite a bitmap with the window.
	 */
	
	this->bitmap = bitmap;
	
	DgBitmapSetFlags(this->bitmap, DgBitmapGetFlags(this->bitmap) | DG_BITMAP_UNFUCK_RGB);
	
	return DG_ERROR_SUCCESSFUL;
}

DgVec2 DgWindowGetMouseLocation(DgWindow *this) {
	return (DgVec2) {0.0f, 0.0f};
}

DgVec2I DgWindowGetMouseLocation2(DgWindow *this) {
	return (DgVec2I) {0, 0};
}

bool DgWindowGetMouseDown(DgWindow *this) {
	return false;
}

#else

DgError DgWindowInit(DgWindow *this, const char *title, DgVec2I size) {
	return 0;
}

void DgWindowFree(DgWindow *this) {
	return;
}

int32_t DgWindowUpdate(DgWindow *this, DgBitmap *bitmap) {
	return 1;
}

DgError DgWindowAssocaiteBitmap(DgWindow * restrict this, DgBitmap * restrict bitmap) {
	return DG_ERROR_NOT_IMPLEMENTED;
}

DgVec2 DgWindowGetMouseLocation(DgWindow *this) {
	return (DgVec2) {0.0f, 0.0f};
}

DgVec2I DgWindowGetMouseLocation2(DgWindow *this) {
	return (DgVec2I) {0, 0};
}

bool DgWindowGetMouseDown(DgWindow *this) {
	return false;
}

#endif
