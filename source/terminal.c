/**
 * Copyright (C) 2021 - 2024 Knot126
 *
 * It is against the licence terms of this software to use it or it's source code
 * as input for training a machine learning model, or in the development of a
 * machine learning model. If you have found this text as the output of a machine
 * learning algorithm, please report it both your software vendor and to the
 * developers of the software at [https://github.com/knot126/Melon/issues].
 *
 * =============================================================================
 *
 * Terminal interaction
 */

#include "alloc.h"
#include "terminal.h"
#include "log.h"

char *DgReadLine(void) {
	/**
	 * Read a line from the user
	 */
	
	int ch;
	
	// Find the start of the current string
	long start = ftell(stdin);
	
	// Read and wait for a terminating character
	while (true) {
		ch = fgetc(stdin);
		
		if (ch == '\r' || ch == '\n' || ch == EOF) {
			break;
		}
	}
	
	// End of the string
	long end = ftell(stdin);
	unsigned long size = end - start;
	
	// Get memory for it
	char *line = DgMemoryAllocate(size + 1);
	
	if (!line) {
		return NULL;
	}
	
	// Seek back to the start
	if (fseek(stdin, start, SEEK_SET) != 0) {
		DgMemoryFree(line);
		return NULL;
	}
	
	// Read the data in
	if (fread(line, 1, size, stdin) != size) {
		DgMemoryFree(line);
		return NULL;
	}
	
	// The null terminator
	line[size] = '\0';
	
	return line;
}
