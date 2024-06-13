/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * Thread abstraction
 */

#ifndef _WIN32
	#include <pthread.h>
#endif

// First two typedefs may change depending on threading library
typedef void *DgThreadArg;
typedef void *DgThreadReturn;

typedef DgThreadReturn (*DgThreadFunction)(DgThreadArg);

typedef struct DgThread {
#ifndef _WIN32
	pthread_t _info;
#else
	int _info;
#endif
} DgThread;

int DgThreadNew(DgThread* thread, DgThreadFunction func, DgThreadArg arg);
int DgThreadJoin(DgThread* thread);
