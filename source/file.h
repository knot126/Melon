/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * Load and save file functions
 */

#pragma once

#include "storage.h"
#include "error.h"

DgError DgFileLoad(DgStorage *storage, DgStoragePath path, size_t *size, void **buffer);
DgError DgFileSave(DgStorage *storage, DgStoragePath path, size_t size, void *buffer);
DgError DgFileAppend(DgStorage *storage, DgStoragePath path, size_t size, void *buffer);
