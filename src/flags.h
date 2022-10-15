/**
 * Copyright (C) 2021 - 2022 Knot126
 * =================================
 * 
 * Bit flag feilds
 */

#pragma once

#include <stdbool.h>
#include <inttypes.h>

typedef uint64_t DgFlags;

DgFlags DgFlagSet(DgFlags feild, DgFlags flag);
DgFlags DgFlagUnset(DgFlags feild, DgFlags flag);
DgFlags DgFlagToggle(DgFlags feild, DgFlags flag);
bool DgFlagCheckOr(DgFlags feild, DgFlags flags);
bool DgFlagCheckAnd(DgFlags feild, DgFlags flags);
