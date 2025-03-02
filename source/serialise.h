/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Serialisation for tables (custom binary format)
 */

#pragma once

#include "table.h"

DgError DgSerialiseWrite(DgStream *stream, DgValue * restrict value);
