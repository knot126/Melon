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
 * Command-Line Argument Parsing
 */ 

#pragma once

typedef struct DgArgPair {
	char *key;
	char *value;
} DgArgPair;

typedef struct DgArgs {
	DgArgPair *pairs;
	size_t     pairs_count;
} DgArgs;

void DgArgParse(DgArgs * restrict this, const size_t argc, char ** const restrict argv);
void DgArgFree(DgArgs * restrict this);

bool DgArgGetFlag(DgArgs * restrict this, const char * const restrict flag);
const char *DgArgGetValue(DgArgs * restrict this, const char * const restrict flag);
const char *DgArgGetValue2(DgArgs * restrict this, const char * const restrict flag, const char * const restrict fallback);
void DgArgPrint(DgArgs * restrict this);
