/*
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * Dictionary - Hopefully the last implementation!
 */

#pragma once

#include <inttypes.h>

typedef enum {
	DG_VALUETYPE_INTEGER = 1,
	DG_VALUETYPE_NUMBER = 4,
	DG_VALUETYPE_STRING = 5,
	DG_VALUETYPE_ARRAY = 6,
	DG_VALUETYPE_OBJECT = 7,
} DgValueType;

struct DgValue;

typedef struct {
	struct DgValue *values;
	size_t length;
} DgValueArray;

struct DgObject;

typedef struct DgValue {
	DgValueType type;
	union {
		int64_t asInteger;
		double asNumber;
		char *asString;
		DgValueArray *asArray;
		struct DgObject *asObject;
	} data;
} DgValue;

typedef struct {
	DgValue key;
	DgValue value;
} DgObjectPair;

typedef struct DgObject {
	DgObjectPair *pairs;
	size_t length;
} DgObject;

uint8_t DgObjectInit(DgValue *val);
uint8_t DgArrayInit(DgValue *val);
DgValue DgNumberNew(double number);
DgValue DgIntegerNew(int64_t number);
