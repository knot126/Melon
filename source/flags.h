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
