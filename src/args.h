/*
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
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
