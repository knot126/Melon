/**
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * Command-Line Argument Parsing
 * 
 * @todo Make it not global state because it has no reason to be this way.
 */ 

#include <stdbool.h>
#include <string.h>

#include "alloc.h"
#include "str.h"
#include "log.h"

#include "args.h"

void DgArgParse(DgArgs * restrict this, const int argc, char ** const restrict argv) {
	/**
	 * Parse the given argumets into the global arguments structure. This could
	 * be changed to use a non-global object, but I think a protected strcture
	 * is enough for arguments - which are basically global state anyways.
	 * 
	 * @param argc Number of arguments to parse
	 * @param argv Array of arguments
	 */
	
	// NOTE: We start at the first real argument, not the name of the exec...
	for (size_t i = 1; i < argc; i++) {
		char *next = argv[i];
		char *value = NULL;
		
		i++;
		
		// This is a key/value argument
		// If this test fails, the value will be null and we will go back to the
		// current argument. This is exactly what is wanted, since a flag can 
		// just be a key without any value.
		if (i < argc && argv[i][0] != '-') {
			value = argv[i];
		}
		else {
			i--;
		}
		
		this->pairs_count++;
		this->pairs = DgRealloc(this->pairs, sizeof(DgArgPair) * this->pairs_count);
		
		if (!this->pairs) {
			DgLog(DG_LOG_ERROR, "Memory allocation error whilst parsing command line arguments.");
			return;
		}
		
		// skip the initial dashes for an argument
		next += DgStrspn(next, "-");
		
		this->pairs[this->pairs_count - 1].key = DgStrdup(next);
		// strdup(NULL) is undefined behaviour, so we need to do a test.
		this->pairs[this->pairs_count - 1].value = (value) ? DgStrdup(value) : NULL;
	}
}

void DgArgFree(DgArgs * restrict this) {
	/**
	 * Free the command line arguments list.
	 */
	
	if (this->pairs) {
		for (size_t i = 0; i < this->pairs_count; i++) {
			if (this->pairs[i].key) {
				DgFree(this->pairs[i].key);
			}
			
			if (this->pairs[i].value) {
				DgFree(this->pairs[i].value);
			}
		}
		
		DgFree(this->pairs);
	}
}

bool DgArgGetFlag(DgArgs * restrict this, const char * const restrict flag) {
	/**
	 * Return true if a flag exsists, false otherwise.
	 * 
	 * @param flag Name of the argument to check for
	 * @return If the flags exists or not
	 */
	
	for (size_t i = 0; i < this->pairs_count; i++) {
		if (!strcmp(this->pairs[i].key, flag)) {
			return true;
		}
	}
	
	return false;
}

const char *DgArgGetValue(DgArgs * restrict this, const char * const restrict flag) {
	/**
	 * Return pointer to a string value if the flag exsits and has a value, 
	 * otherwise returns NULL.
	 * 
	 * @param flag Name of the argument to check for
	 * @return String of the argument content
	 */
	
	for (size_t i = 0; i < this->pairs_count; i++) {
		if (!strcmp(this->pairs[i].key, flag)) {
			return this->pairs[i].value;
		}
	}
	
	return NULL;
}

const char *DgArgGetValue2(DgArgs * restrict this, const char * const restrict flag, const char * const restrict fallback) {
	/**
	 * The the value of flag, otherwise return a fallback, even if the argument
	 * exsits.
	 * 
	 * @param flag Name of the argument to look for
	 * @param fallback Fallback value to return if not found
	 * @return String of argument content
	 * 
	 * @see DgArgGetValue
	 */
	
	char *ptr = NULL;
	
	for (size_t i = 0; i < this->pairs_count; i++) {
		if (!strcmp(this->pairs[i].key, flag)) {
			ptr = this->pairs[i].value;
			break;
		}
	}
	
	if (ptr) {
		return ptr;
	}
	else {
		return fallback;
	}
}

void DgArgPrint(DgArgs * restrict this) {
	/**
	 * Print loaded arguments
	 * 
	 * @warning Only use after arguments have been initialised
	 */
	
	for (size_t i = 0; i < this->pairs_count; i++) {
		DgLog(DG_LOG_VERBOSE, "arg %d = %s -> %s", i, this->pairs[i].key, this->pairs[i].value);
	}
}
