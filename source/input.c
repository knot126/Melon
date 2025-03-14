/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Input event processing
 */

#include "memory.h"
#include "time.h"

#include "input.h"

void DgInputInit(DgInput *this) {
	/**
	 * Initialise input state
	 */
	
	this->repeatTime = 0.7f;
	
	for (size_t i = 0; i < 128; i++) {
		this->key_press_time[i] = DG_INPUT_NOT_PRESSED_TIME;
	}
	
	memset(this->key_pressed, 0, 128);
}

void DgInputFrame(DgInput *this) {
	/**
	 * Update the state per frame
	 */
	
	memset(this->key_pressed, 0, 128);
}

void DgInputKeyPressEvent(DgInput *this, uint32_t codepoint) {
	/**
	 * Register a key press event
	 */
	
	if (codepoint < 128) {
		this->key_press_time[codepoint] = DgRealTime();
		this->key_pressed[codepoint] = true;
	}
}

void DgInputKeyReleaseEvent(DgInput *this, uint32_t codepoint) {
	/**
	 * Register a key release event
	 */
	
	if (codepoint < 128) {
		this->key_press_time[codepoint] = DG_INPUT_NOT_PRESSED_TIME;
	}
}

bool DgInputIsKeyDown(DgInput *this, uint32_t codepoint) {
	return this->key_press_time[codepoint] > 0.0f;
}

bool DgInputIsKeyUp(DgInput *this, uint32_t codepoint) {
	return this->key_press_time[codepoint] == DG_INPUT_NOT_PRESSED_TIME;
}

DgInputStatus DgInputIsKeyPressedEx(DgInput *this, uint32_t codepoint, bool consume, bool repeat, double repeatTime) {
	if (this->key_pressed[codepoint] == true) {
		if (consume) {
			this->key_pressed[codepoint] = false;
		}
		
		return DG_INPUT_TRUE;
	}
	
	return (repeat && (DgRealTime() > this->key_press_time[codepoint] + repeatTime)) ? DG_INPUT_REPEATED : DG_INPUT_FALSE;
}

bool DgInputIsKeyPressed(DgInput *this, uint32_t codepoint) {
	return DgInputIsKeyPressedEx(this, codepoint, true, true, this->repeatTime) != DG_INPUT_FALSE;
}
