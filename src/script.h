/*
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
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
