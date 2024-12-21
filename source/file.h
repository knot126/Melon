/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * Load and save file functions
 */

#pragma once

#include "stream.h"
#include "error.h"

DgError DgFileOpen(DgStream *stream, const char *path, DgStreamOpenFlags flags);
DgError DgFileLoad(const char *path, size_t *size, void **buffer);
