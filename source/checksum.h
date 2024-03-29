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
 * Hashing Strings and Data
 */ 

#pragma once

#include <inttypes.h>

uint32_t DgChecksumStringU32_DJB2(const char * str);
uint32_t DgChecksumU32_DJB2(size_t length, const char *data);
uint32_t DgChecksumStringU32(const char * str);
