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
 * Command line argument parsing
 */ 

#include <stdbool.h>
#include <string.h>

#include "alloc.h"
#include "str.h"
#include "log.h"

#include "args.h"

DgError DgArgParse(DgArgs * restrict this, const size_t argc, char ** const restrict argv) {
	/**
	 * Parse the given argumets into the given arguments structure.
	 * 
	 * @note This supports:
	 *     -k ('-' {single letter})
	 *     --key ('--' {key name})
	 *     --key value ('--' {key name} ' ' {key value})
	 * 
	 * @todo Support for '--key=value' (note: '--key' 'value' works)
	 * 
	 * @param this Arguments object
	 * @param argc Number of arguments to parse
	 * @param argv Array of arguments
	 */
	
	memset(this, 0, sizeof *this);
	
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
			return DG_ERROR_ALLOCATION_FAILED;
		}
		
		// skip the initial dashes for an argument
		// next += DgStrspn(next, "-"); (Changed because this function was deprecated)
		while (next[0] == '-') {
			next++;
		}
		
		this->pairs[this->pairs_count - 1].key = DgStringDuplicate(next);
		// strdup(NULL) is undefined behaviour, so we need to do a test.
		this->pairs[this->pairs_count - 1].value = (value) ? DgStringDuplicate(value) : NULL;
	}
	
	return DG_ERROR_SUCCESS;
}

void DgArgFree(DgArgs * restrict this) {
	/**
	 * Free the command line arguments list.
	 * 
	 * @param this Arguments object
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
	 * @param this Arguments object
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
	 * @param this Arguments object
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
	 * @param this Arguments object
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
	
	return (ptr) ? (ptr) : (fallback);
}

void DgArgPrint(DgArgs * restrict this) {
	/**
	 * Print loaded arguments
	 * 
	 * @param this Arguments object
	 */
	
	for (size_t i = 0; i < this->pairs_count; i++) {
		DgLog(DG_LOG_VERBOSE, "arg %d = %s -> %s", i, this->pairs[i].key, this->pairs[i].value);
	}
}
