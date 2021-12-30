/*
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * Hashing Strings and Data
 */ 

#pragma once

#include <inttypes.h>

uint32_t DgHashStringU32_366(const char * str);
uint32_t DgHashStringU32_DJB2(const char * str);
uint32_t DgHashStringU32(const char * str);
