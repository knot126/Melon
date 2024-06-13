/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * Serialisation for tables (custom binary format)
 */

#pragma once

#include "table.h"

DgError DgSerialiseWrite(DgStorage *storage, const char *path, DgValue * restrict value);
