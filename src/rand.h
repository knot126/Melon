/*
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * Random Numbers and Strings
 */

#pragma once

#include <inttypes.h>
#include <stdbool.h>

uint32_t DgRandInt(void);
float DgRandFloat(void);
bool DgRandBool(void);

#if !defined(DG_NO_LUA)

#include "script.h"

void DgRegisterRandFuncs(DgScript *script);

#endif
