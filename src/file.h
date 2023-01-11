/**
 * Copyright (C) 2021 - 2023 Knot126 and Descentix Software
 * ========================================================
 * 
 * Load and save file functions
 */

#pragma once

#include "storage.h"
#include "error.h"

DgError DgFileLoad(DgStorage *storage, DgStoragePath path, size_t *size, void **buffer);
DgError DgFileSave(DgStorage *storage, DgStoragePath path, size_t size, void *buffer);
DgError DgFileAppend(DgStorage *storage, DgStoragePath path, size_t size, void *buffer);
