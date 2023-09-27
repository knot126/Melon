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
 * Scripting Support Utilities
 */

#pragma once

#if !defined(DG_NO_LUA)

#include <stdbool.h>
#include <inttypes.h>

#include "lua.h"

typedef struct {
	lua_State *state;
} DgScript;

enum {
	DG_SCRIPT_NIL = 0,
	DG_SCRIPT_NUMBER = 1,
	DG_SCRIPT_STRING = 3,
	DG_SCRIPT_BOOLEAN = 4,
};

void DgScriptInit(DgScript *script);
bool DgScriptLoad(DgScript *script, char * const path);
void DgScriptCall(DgScript *script, char *name);
void DgScriptCallArgs(DgScript *script, char *name, int *types, int argc, ...);
void DgScriptFree(DgScript *script);
void DgScriptRegister(DgScript *script, const char * name, int (*function)(DgScript *));

#endif
