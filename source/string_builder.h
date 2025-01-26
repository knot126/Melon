/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * String building utilities
 */

#pragma once

#include "common.h"

// The maximum length of a string builder string before allocations are
// needed
// NOTE: At the moment this is the max size of a string builder string, but in
// the future it should start dynamically allocating memory.
#define DG_STRING_BUILDER_LOCAL_LENGTH 0x1000

typedef struct DgStringBuilder {
	size_t head;
	char local[DG_STRING_BUILDER_LOCAL_LENGTH];
} DgStringBuilder;

void DgStringBuilderClear(DgStringBuilder *this);
bool DgStringBuilderAppendBytes(DgStringBuilder *this, size_t size, const void *data);
bool DgStringBuilderAppendChar(DgStringBuilder *this, char ch);
bool DgStringBuilderAppend(DgStringBuilder *this, const char *data);
const char *DgStringBuilderGetTemp(DgStringBuilder *this);
char *DgStringBuilderGet(DgStringBuilder *this);
