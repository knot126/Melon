/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Input event processing
 */

#pragma once

#include "common.h"

#define DG_INPUT_NOT_PRESSED_TIME -1.0f

typedef enum : uint8_t {
	DG_INPUT_FALSE = 0,
	DG_INPUT_TRUE = 1,
	DG_INPUT_REPEATED = 2,
} DgInputStatus;

typedef struct DgInput {
	double repeatTime; // Time before key repeats, if allowed
	double key_press_time[128]; // Times keys went down
	bool key_pressed[128]; // If the keys are considered pressed (e.g. if any event has registered their press)
} DgInput;

void DgInputInit(DgInput *this);
void DgInputFrame(DgInput *this);

// Pushing new events
void DgInputKeyPressEvent(DgInput *this, uint32_t codepoint);
void DgInputKeyReleaseEvent(DgInput *this, uint32_t codepoint);

bool DgInputIsKeyDown(DgInput *this, uint32_t codepoint);
bool DgInputIsKeyUp(DgInput *this, uint32_t codepoint);
DgInputStatus DgInputIsKeyPressedEx(DgInput *this, uint32_t codepoint, bool consume, bool repeat, double repeatTime);
bool DgInputIsKeyPressed(DgInput *this, uint32_t codepoint);
