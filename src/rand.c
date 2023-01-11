/*
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * Random Numbers and Strings
 */

#include <math.h>
#include <inttypes.h>
#include <stdbool.h>

#include "time.h"

#include "rand.h"

uint32_t DgRandXORShiftU32(uint32_t n) {
	/**
	 * Generate a random number based on a given seed using the XOR-Shift 
	 * method of generating a random number.
	 */
	
	n ^= n << 13;
	n ^= n >> 15;
	n ^= n << 5;
	
	return n;
}

uint32_t DgRandXORShiftSU32(void) {
	/**
	 * A pesudostateless version of the XOR-shift function. It should be 
	 * semi-threadsafe, since it will add a (most likely) unique value to make
	 * sure there are minimal conflicts.
	 */
	
	static uint32_t last;
	
	last = DgRandXORShiftU32(last + DgNsecTime());
	
	return last;
}

float DgRandXORShiftF32(void) {
	return ((float) DgRandXORShiftSU32()) / 4294967295.0f;
}

uint32_t DgRandInt(void) {
	return DgRandXORShiftSU32();
}

float DgRandFloat(void) {
	return DgRandXORShiftF32();
}

bool DgRandBool(void) {
	return (DgRandXORShiftSU32() % 2) == 1;
}

#if !defined(DG_NO_LUA)

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "script.h"

static int DgRandInt_Scripted(lua_State *script) {
	lua_pushinteger(script, DgRandInt());
	
	return 1;
}

static int DgRandFloat_Scripted(lua_State *script) {
	lua_pushnumber(script, DgRandFloat());
	
	return 1;
}

static int DgRandBool_Scripted(lua_State *script) {
	lua_pushboolean(script, DgRandBool());
	
	return 1;
}

void DgRegisterRandFuncs(DgScript *script) {
	lua_register(script->state, "mgRandInt", &DgRandInt_Scripted);
	lua_register(script->state, "mgRandFloat", &DgRandFloat_Scripted);
	lua_register(script->state, "mgRandBool", &DgRandBool_Scripted);
	
	lua_register(script->state, "rand_int", &DgRandInt_Scripted);
	lua_register(script->state, "rand_num", &DgRandFloat_Scripted);
	lua_register(script->state, "rand_bool", &DgRandBool_Scripted);
}

#endif
