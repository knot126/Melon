/**
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * Generic array macros
 */

#pragma once

#include <inttypes.h>
#include <stddef.h>
#include <string.h>
#include "util/alloc.h"

enum {
	DG_ARRAY_SUCCESS = 0,
	DG_ARRAY_ERROR_FATAL = -1,
	DG_ARRAY_ERROR_USAGE = -2,
};

/**
 * Create the implementation of the array.
 */
#define DG_CREATE_ARRAY(NAME, TYPE, SZ_TYPE) \
typedef struct NAME ## Array {\
	TYPE *data;\
	SZ_TYPE count, alloc;\
} NAME ## Array;\
\
uint32_t NAME ## ArrayInit(NAME ## Array *this, SZ_TYPE prealloc);\
uint32_t NAME ## ArrayAppend(NAME ## Array *this, TYPE *appendend);\
uint32_t NAME ## ArrayRemove(NAME ## Array *this, SZ_TYPE index);\
void NAME ## ArrayFree(NAME ## Array *this, void (*freefunc)(TYPE *memory));

/**
 * Create the header of the array.
 * 
 * NOTE: You cannot use comments in preprocessor directives. They will break badly.
 */
#define DG_CREATE_ARRAY_DEFS(NAME, TYPE, SZ_TYPE) \
uint32_t NAME ## ArrayInit(NAME ## Array *this, SZ_TYPE prealloc) {\
	memset(this->data, 0, sizeof *this->data * this->count);\
	\
	this->alloc = prealloc;\
	\
	if (prealloc > 0) {\
		this->data = DgAlloc(sizeof *this->data * this->count);\
		\
		if (!this->data) {\
			return DG_ARRAY_ERROR_FATAL;\
		}\
	}\
	\
	return DG_ARRAY_SUCCESS;\
}\
\
static uint32_t NAME ## ArrayRealloc(NAME ## Array *this) {\
	if (this->count >= this->alloc) {\
		this->alloc = 2 + (this->alloc * 2);\
		this->data = DgRealloc(this->data, sizeof *this->data * this->alloc);\
		\
		if (!this->data) {\
			return DG_ARRAY_ERROR_FATAL;\
		}\
	}\
	\
	return DG_ARRAY_SUCCESS;\
}\
\
uint32_t NAME ## ArrayAppend(NAME ## Array *this, TYPE *appendend) {\
	if (NAME ## ArrayRealloc(this) != DG_ARRAY_SUCCESS) {\
		return DG_ARRAY_ERROR_FATAL;\
	}\
	\
	this->data[this->count++] = *appendend;\
	\
	return DG_ARRAY_SUCCESS;\
}\
\
uint32_t NAME ## ArrayRemove(NAME ## Array *this, SZ_TYPE index) {\
	if (index > this->count) {\
		return DG_ARRAY_ERROR_USAGE;\
	}\
	\
	if (index != this->count - 1) {\
		memmove(&this->data[index], &this->data[index + 1], (this->count - 1) - index);\
	}\
	\
	return DG_ARRAY_SUCCESS;\
}\
\
void NAME ## ArrayFree(NAME ## Array *this, void (*freefunc)(TYPE *memory)) {\
	if (freefunc) {\
		for (size_t i = 0; i < this->count; i++) {\
			freefunc(&this->data[i]);\
		}\
	}\
	\
	this->count = 0;\
	this->alloc = 0;\
	\
	DgFree(this->data);\
	\
	this->data = NULL;\
};
