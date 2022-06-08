/**
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * Dictionary - Hopefully the last implementation!
 * 
 * @deprecated Never used or tested
 */

#include <string.h>

#include "alloc.h"

#include "value.h"

uint8_t DgObjectInit(DgValue *val) {
	/**
	 * Initialise an object given a value container. Returns 0 on success.
	 */
	
	DgObject *obj = (DgObject *) DgAlloc(sizeof(DgObject));
	
	if (!obj) {
		return 1;
	}
	
	memset(obj, 0, sizeof(DgObject));
	val->type = DG_VALUETYPE_OBJECT;
	val->data.asObject = obj;
	
	return 0;
}

uint8_t DgArrayInit(DgValue *val) {
	/**
	 * Initialise an array given a value container. Returns 0 on success.
	 */
	
	DgValueArray *arr = (DgValueArray *) DgAlloc(sizeof(DgValueArray));
	
	if (!arr) {
		return 1;
	}
	
	memset(arr, 0, sizeof(DgValueArray));
	val->type = DG_VALUETYPE_ARRAY;
	val->data.asArray = arr;
	
	return 0;
}

DgValue DgNumberNew(double number) {
	DgValue num;
	
	memset(&num, 0, sizeof(DgValue));
	
	num.type = DG_VALUETYPE_NUMBER;
	num.data.asNumber = number;
	
	return num;
}

DgValue DgIntegerNew(int64_t number) {
	DgValue num;
	
	memset(&num, 0, sizeof(DgValue));
	
	num.type = DG_VALUETYPE_INTEGER;
	num.data.asInteger = number;
	
	return num;
}
