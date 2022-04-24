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

#if !defined(DG_NO_SDL)
	#include <SDL2/SDL.h>
#endif

#include "maths.h"
#include "bitmap.h"

#include "window.h"

#if !defined(DG_NO_SDL)

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
	uint32_t *wd = (uint32_t *) this->surface->pixels;
	uint32_t pixel_count = this->size.x * this->size.y;
	uint8_t *data = bitmap->src;
	
	for (size_t i = 0; i < pixel_count; i++) {
		wd[i] = SDL_MapRGBA(this->surface->format, data[(i * 3)], data[(i * 3) + 1], data[(i * 3) + 2], 0xff);
	}
	
	return SDL_UpdateWindowSurface(this->window) ? 1 : 0;
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

#endif
