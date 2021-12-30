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

void DgArgParse(const int argc, char ** const restrict argv);
void DgArgFree(void);

bool DgArgGetFlag(const char * const restrict flag);
const char *DgArgGetValue(const char * const restrict flag);
const char *DgArgGetValue2(const char * const restrict flag, const char * const restrict fallback);
void DgArgPrint(void);
