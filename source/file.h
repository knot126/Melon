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
 * Load and save file functions
 */

#pragma once

#include "storage.h"
#include "error.h"

DgError DgFileLoad(DgStorage *storage, DgStoragePath path, size_t *size, void **buffer);
DgError DgFileSave(DgStorage *storage, DgStoragePath path, size_t size, void *buffer);
DgError DgFileAppend(DgStorage *storage, DgStoragePath path, size_t size, void *buffer);
