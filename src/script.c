/*
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * Scripting Support Utilities
 */

#if !defined(DG_NO_LUA)

#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "alloc.h"
#include "fs.h"

#include "script.h"

void DgScriptInit(DgScript *script) {
	/*
	 * Initialise the script
	 */
	
	memset(script, 0, sizeof(DgScript));
	
	script->state = luaL_newstate();
	
	luaL_openlibs(script->state);
}

bool DgScriptLoad(DgScript *script, char * const path) {
	/*
	 * Load a script file.
	 */
	
	char * real_path = DgEvalPath(path);
	
	if (!real_path) {
		return false;
	}
	
	int stat = luaL_dofile(script->state, real_path);
	
	DgFree(real_path);
	
	if (stat) {
		printf("\033[1;31mError:\033[0m Error whilst doing file %s, got errorcode %d.\n", path, stat);
		return false;
	}
	
	return true;
}

void DgScriptCall(DgScript *script, char *name) {
	/*
	 * Call a function in a script that has no arguments and no return values.
	 */
	
	int type = lua_getglobal(script->state, name);
	
	if (type == LUA_TNIL) {
		return;
	}
	
	lua_call(script->state, 0, 0);
	
	lua_pop(script->state, 0);
}

void DgScriptCallArgs(DgScript *script, char *name, int *types, int argc, ...) {
	/*
	 * Call a function in a script that has any number and type of argumets but 
	 * no return values.
	 * 
	 * ´argc´ must be the count of arguments, and ´types´ must be an int array
	 * of ´argc´ length that contains an array of the types for each value
	 * in order.
	 */
	
	int type = lua_getglobal(script->state, name);
	
	if (type == LUA_TNIL) {
		return;
	}
	
	va_list args;
	va_start(args, argc);
	
	for (int i = 0; i < argc; i++) {
		switch (types[i]) {
			case DG_SCRIPT_NUMBER: lua_pushnumber(script->state, va_arg(args, double)); break;
			case DG_SCRIPT_STRING: lua_pushstring(script->state, va_arg(args, char *)); break;
			case DG_SCRIPT_BOOLEAN: lua_pushboolean(script->state, va_arg(args, int)); break;
			default: lua_pushnil(script->state); break;
		}
	}
	
	va_end(args);
	
	lua_call(script->state, argc, 0);
	
	lua_pop(script->state, 0);
}

void DgScriptFree(DgScript *script) {
	/*
	 * Free script resources
	 */
	
	lua_close(script->state);
}

void DgScriptRegister(DgScript *script, const char * name, int (*function)(DgScript *)) {
	/**
	 * Register a C function of type (int (*)(DgScript *)) for calling.
	 */
	
	lua_register(script->state, name, (int (*)(lua_State *)) function);
}

#endif //!defined(DG_NO_LUA)
