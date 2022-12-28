/**
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
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
#include "rand.h" // TODO temp

#include "window.h"

#if !defined(DG_NO_SDL)

#include <SDL2/SDL.h>

uint32_t gWindowCount_ = 0;

uint32_t DgWindowInit(DgWindow *this, const char *title, DgVec2I size) {
	/**
	 * Initialise and create a window
	 * 
	 * @param this Window object
	 * @return Zero on success, non-zero on failure
	 */
	
	gWindowCount_++;
	
	if (!SDL_WasInit(0)) {
		SDL_Init(SDL_INIT_VIDEO);
	}
	
	this->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.x, size.y, 0);
	this->surface = SDL_GetWindowSurface(this->window);
	this->size = size;
	
	return 0;
}

void DgWindowFree(DgWindow *this) {
	/**
	 * Free resources assocaited with a window
	 * 
	 * @param this Window object
	 */
	
	gWindowCount_--;
	
	if (gWindowCount_ == 0) {
		SDL_Quit();
	}
}

int32_t DgWindowUpdate(DgWindow *this, DgBitmap *bitmap) {
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
	 * @return Zero on success, less than zero on failure, one on should exit
	 */
	
	// Check up on events
	SDL_Event event;
	
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT: {
				return 1;
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
	
	return SDL_UpdateWindowSurface(this->window) ? 1 : 0;
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
	
	DgBitmapSetSource(bitmap, this->surface->pixels, this->size, 4);
	
	return DG_ERROR_SUCCESSFUL;
}

DgVec2 DgWindowGetMouseLocation(DgWindow * restrict this) {
	/**
	 * Return the current mouse position.
	 * 
	 * @param this Window object
	 * @return Cursor position relative to window
	 */
	
	int x, y;
	
	SDL_PumpEvents();
	SDL_GetMouseState(&x, &y);
	
	return (DgVec2) {(float)(x) / (float)(this->size.x), ((float)(y) / (float)(this->size.y))};
}

DgVec2I DgWindowGetMouseLocation2(DgWindow * restrict this) {
	/**
	 * Return the current mouse position in window coordinates.
	 * 
	 * @param this Window object (not needed using SDL backend)
	 * @return Cursor position relative to window in window coordinates
	 */
	
	int x, y;
	
	SDL_PumpEvents();
	SDL_GetMouseState(&x, &y);
	
	return (DgVec2I) {x, y};
}

bool DgWindowGetMouseDown(DgWindow * restrict this) {
	/**
	 * Get if the left mouse button is down or not.
	 * 
	 * @param this Window object (not needed using SDL backend)
	 * @return Cursor position relative to window
	 */
	
	SDL_PumpEvents();
	return !!(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK);
}

#elif defined(DG_USE_WINDOWS_API)

#include <windows.h>

const char DG_WINDOW_CLASS_NAME[] = "Melon Library Window";

static LRESULT CALLBACK DgWindow_NTNopWindowCallback(HWND window_handle, UINT message_type, WPARAM wparam, LPARAM lparam) {
	switch (message_type) {
		case WM_PAINT: {
			return 0;
		}
		
		default: {
			DgLog(DG_LOG_INFO, "DefWindowProc %d", message_type);
			return DefWindowProc(window_handle, message_type, wparam, lparam);
		}
	}
}

uint32_t DgWindowInit(DgWindow *this, const char *title, DgVec2I size) {
	/**
	 * Create a window (Windows NT)
	 * 
	 * @param this Handle to the window object
	 * @param title Title of the window
	 * @param size Size of the window
	 */
	
	// Setup window class
	memset(&this->window_class, 0, sizeof this->window_class);
	this->window_class.lpfnWndProc = &DgWindow_NTNopWindowCallback; // TODO: need to be the callback
	this->window_class.hInstance = GetModuleHandle(NULL); // It seems this can be NULL and it will take care of things.
	this->window_class.lpszClassName = DG_WINDOW_CLASS_NAME;
	
	// Register the class
	if (RegisterClass(&this->window_class) == 0) {
		DgLog(DG_LOG_VERBOSE, "RegisterClass: error code = %d", GetLastError());
		return 1;
	}
	
	this->window_handle = CreateWindowEx(
		0,
		DG_WINDOW_CLASS_NAME,
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		size.x, size.y,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL
	);
	
	if (this->window_handle == NULL) {
		DgLog(DG_LOG_VERBOSE, "CreateWindowEx: error code = %d", GetLastError());
		return 1;
	}
	
	// Pop up the window
	ShowWindow(this->window_handle, SW_SHOW);
	
	return 0;
}

void DgWindowFree(DgWindow *this) {
	return;
}

int32_t DgWindowUpdate(DgWindow *this, DgBitmap *bitmap) {
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
	
	InvalidateRect(this->window_handle, NULL, FALSE);
	
	while (PeekMessage(&message, this->window_handle, 0, 0, PM_NOREMOVE) != 0) {
		// Any extra handles for the message
		switch (message.message) {
			case WM_PAINT: {
				PAINTSTRUCT painter;
				
				HDC context = BeginPaint(this->window_handle, &painter);
				
				FillRect(context, &painter.rcPaint, (HBRUSH) (COLOR_WINDOW + 1 + (DgRandInt() & 0xf)));
				
				EndPaint(this->window_handle, &painter);
				
				return 0;
			}
			
			case WM_CLOSE:
			case WM_DESTROY:
			case WM_QUIT: {
				DgLog(DG_LOG_INFO, "** Destroy window message **");
				return 1;
			}
		}
		
		// Remove message
		PeekMessage(&message, this->window_handle, 0, 0, PM_REMOVE);
		
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	
	DgLog(DG_LOG_INFO, "Exit window update");
	
	return 0;
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

#else

uint32_t DgWindowInit(DgWindow *this, const char *title, DgVec2I size) {
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
