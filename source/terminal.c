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

#include "bytes.h"
#include "terminal.h"
#include "log.h"

char *DgReadLine(const char *prompt) {
	/**
	 * Read a line from the user
	 * 
	 * @todo make it work!!!
	 */
	
	DgError error;
	DgBytes line;
	int ch;
	
	DgBytesInit(&line);
	
	printf("%s", prompt);
	
	// Read and wait for a terminating character
	while (true) {
		// Get the next character from the user
		ch = fgetc(stdin);
		
		// If it's a terminal character, stop reading
		if (ch == '\r' || ch == '\n' || ch == EOF) {
			break;
		}
		
		// Place the char into the buffer
		error = DgBytesAppendByte(&line, ch);
		
		if (error) {
			return NULL;
		}
	}
	
	// Append a NUL byte
	error = DgBytesAppendByte(&line, '\0');
	
	if (error) {
		return NULL;
	}
	
	// Convert to string
	char *out;
	DgBytesToBuffer(&line, (DgByte **) &out, NULL);
	
	return out;
}
