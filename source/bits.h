/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Memory and bit-level access
 */

#pragma once

#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

bool DgBitRead(void *base, size_t bit);
uint64_t DgBitsRead(void *base, size_t bits, size_t count);
