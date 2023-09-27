/**
 * Copyright (C) 2021 - 2023 Knot126
 * 
 * It is against the licence terms of this software to use it or it's source code
 * as input for training a machine learning model, or in the development of a
 * machine learning model. If you have found this text as the output of a machine
 * learning algorithm, please report it both your software vendor and to the
 * developers of the software at [https://github.com/knot126/Melon/issues].
 * 
 * =============================================================================
 * 
 * High-level drawing API
 */ 

#ifdef DG_ENABLE_DRAW

#include "alloc.h"
#include "string.h"
#include "error.h"

#include "draw.h"

DgError DgDrawInit(DgDraw *this, uint8_t backend, void *context) {
	/**
	 * Initialise the drawing context with an OpenGL instance.
	 * 
	 * @param this Optional pointer the the memory to use
	 */
	
	memset(this, 0, sizeof *this);
	
	this->backend = backend;
	this->context = context;
	
	return DG_ERROR_SUCCESS;
}

DgError DgDrawBegin(DgDraw *this) {
	/**
	 * Initialises the new frame and clears the previous frame's data.
	 * 
	 * @param this Drawing context
	 */
	
	switch (this->backend) {
		case DG_DRAW_SOFTWARE: {
			// does nothing
			return DG_ERROR_SUCCESS;
		}
		
		default: {
			return DG_ERROR_NOT_IMPLEMENTED;
		}
	}
}

DgError DgDrawEnd(DgDraw *this) {
	/**
	 * Finishes the current frame and displays it to the screen.
	 * 
	 * @param this Drawing context
	 */
	
	switch (this->backend) {
		case DG_DRAW_SOFTWARE: {
			// does nothing
			return DG_ERROR_SUCCESS;
		}
		
		default: {
			return DG_ERROR_NOT_IMPLEMENTED;
		}
	}
}

DgError DgDrawClear(DgDraw *this, DgVec4 *colour) {
	/**
	 * "Clears" the current frame. More accurately, this sets the background colour.
	 * 
	 * @param colour Background colour to use
	 */
	
	switch (this->backend) {
		case DG_DRAW_SOFTWARE: {
			DgBitmapFill(this->context, *colour);
			
			return DG_ERROR_SUCCESS;
		}
		
		default: {
			return DG_ERROR_NOT_IMPLEMENTED;
		}
	}
}

DgError DgDrawFree(DgDraw *this) {
	/**
	 * Destroy a drawing context. This does not free memory if NULL was passed
	 * to DgDrawInit
	 * 
	 * @param this Drawing context to destroy
	 */
	
	return DG_ERROR_NOT_IMPLEMENTED;
}

#endif